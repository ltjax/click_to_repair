#if defined(WIN32)
// Onut
#include <onut/Strings.h>

// Private
#include "MFPlayer.h"
#include "RendererD3D11.h"
#include "TextureD3D11.h"

// STL
#include <cassert>
#include <codecvt>
#include <Mfapi.h>

namespace onut
{
    MFPlayerNotify::MFPlayerNotify(const OMFPlayerRef& pPlayer)
        : m_pPlayer(pPlayer)
    {
    }

    HRESULT STDMETHODCALLTYPE MFPlayerNotify::QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject)
    {
        if (!ppvObject)
        {
            return E_INVALIDARG;
        }
        if (riid == IID_IUnknown || riid == __uuidof((IMFMediaEngineNotify*)this))
        {
            *ppvObject = this;
            AddRef();
            return NOERROR;
        }
        return E_NOINTERFACE;
    }

    ULONG STDMETHODCALLTYPE MFPlayerNotify::AddRef(void)
    {
        ++m_ref;
        return m_ref;
    }

    ULONG STDMETHODCALLTYPE MFPlayerNotify::Release(void)
    {
        auto ret = m_ref;
        if (m_ref > 0)
        {
            --m_ref;
            ret = m_ref;
        }
        if (m_ref == 0)
        {
            delete this;
        }
        return ret;
    }

    HRESULT STDMETHODCALLTYPE MFPlayerNotify::EventNotify(_In_ DWORD event, _In_ DWORD_PTR param1, _In_  DWORD param2)
    {
        if (!m_pPlayer.expired())
        {
            m_pPlayer.lock()->OnEvent(event, param1, param2);
        }
        return S_OK;
    }

    void MFPlayer::OnEvent(DWORD event, DWORD_PTR param1, DWORD param2)
    {
        if (event == MF_MEDIA_ENGINE_EVENT_NOTIFYSTABLESTATE)
        {
            SetEvent(reinterpret_cast<HANDLE>(param1));
        }
        else if (event == MF_MEDIA_ENGINE_EVENT_FORMATCHANGE)
        {
            DWORD videoWidth, videoHeight;
            m_pMediaEngine->GetNativeVideoSize(&videoWidth, &videoHeight);
            if (m_pRenderTarget)
            {
                m_pRenderTarget->resizeTarget({static_cast<int>(videoWidth), static_cast<int>(videoHeight)});
            }
        }
        else if (event == MF_MEDIA_ENGINE_EVENT_ENDED ||
                 event == MF_MEDIA_ENGINE_EVENT_ERROR)
        {
            m_isPlaying = false;
        }
    }

    OVideoPlayerRef VideoPlayer::createWithWindowHandle(HWND handle)
    {
        auto pRet = std::make_shared<MFPlayer>();
        pRet->init(handle);
        return pRet;
    }

    OVideoPlayerRef VideoPlayer::createWithRenderTarget(const OTextureRef& pRenderTarget)
    {
        auto pRet = std::make_shared<MFPlayer>();
        pRet->init(pRenderTarget);
        return pRet;
    }

    MFPlayer::~MFPlayer()
    {
        if (m_pDXGIManager)
        {
            m_pDXGIManager->Release();
        }
        if (m_pMediaEngine)
        {
            m_pMediaEngine->Shutdown();
            m_pMediaEngine->Release();
        }
        if (m_pPlayerNodify)
        {
            m_pPlayerNodify->Release();
        }
    }

    void MFPlayer::init(HWND handle)
    {
        HRESULT ret;

        // Initialize M$ bullshit
        //ret = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        //assert(ret == S_OK);
        ret = MFStartup(MF_VERSION);
        assert(ret == S_OK);

        // Create factory
        IMFMediaEngineClassFactory *pMediaEngineClassFactory = nullptr;
        ret = CoCreateInstance(CLSID_MFMediaEngineClassFactory, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pMediaEngineClassFactory));
        assert(ret == S_OK);

        // Create notify
        m_pPlayerNodify = new MFPlayerNotify(shared_from_this());

        // Create attributes
        IMFAttributes *pAttributes = nullptr;
        ret = MFCreateAttributes(&pAttributes, 1);
        assert(ret == S_OK);
        ret = pAttributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, m_pPlayerNodify);
        assert(ret == S_OK);
        ret = pAttributes->SetUINT64(MF_MEDIA_ENGINE_PLAYBACK_HWND, reinterpret_cast<UINT64>(handle));
        assert(ret == S_OK);

        // Create player
        ret = pMediaEngineClassFactory->CreateInstance(0, pAttributes, &m_pMediaEngine);
        assert(ret == S_OK);

        // Release bullshits
        pAttributes->Release();
        pMediaEngineClassFactory->Release();
    }

    void MFPlayer::init(const OTextureRef& pRenderTarget)
    {
        m_pRenderTarget = pRenderTarget;
        auto pRendererD3D11 = std::dynamic_pointer_cast<ORendererD3D11>(oRenderer);

        HRESULT ret;

        // Initialize M$ bullshit
        //ret = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        //assert(ret == S_OK);
        ret = MFStartup(MF_VERSION);
        assert(ret == S_OK);

        // Create factory
        IMFMediaEngineClassFactory *pMediaEngineClassFactory = nullptr;
        ret = CoCreateInstance(CLSID_MFMediaEngineClassFactory, nullptr, CLSCTX_ALL, IID_PPV_ARGS(&pMediaEngineClassFactory));
        assert(ret == S_OK);

        // Create notify
        m_pPlayerNodify = new MFPlayerNotify(shared_from_this());

        // Create attributes
        IMFAttributes *pAttributes = nullptr;
        ret = MFCreateAttributes(&pAttributes, 1);
        assert(ret == S_OK);
        ret = pAttributes->SetUnknown(MF_MEDIA_ENGINE_CALLBACK, m_pPlayerNodify);
        assert(ret == S_OK);

        ID3D10Multithread *pMultithread = nullptr;
        ID3D11Device *pDevice = pRendererD3D11->getDevice();
        ret = pDevice->QueryInterface(IID_PPV_ARGS(&pMultithread));
        assert(ret == S_OK);
        pMultithread->SetMultithreadProtected(TRUE);
        pMultithread->Release();

        UINT resetToken = 0;
        ret = MFCreateDXGIDeviceManager(&resetToken, &m_pDXGIManager);
        assert(ret == S_OK);
        ret = m_pDXGIManager->ResetDevice(pRendererD3D11->getDevice(), resetToken);
        assert(ret == S_OK);
        ret = pAttributes->SetUnknown(MF_MEDIA_ENGINE_DXGI_MANAGER, m_pDXGIManager);
        assert(ret == S_OK);

        ret = pAttributes->SetUINT32(MF_MEDIA_ENGINE_VIDEO_OUTPUT_FORMAT, DXGI_FORMAT_R8G8B8A8_UNORM);
        assert(ret == S_OK);

        // Create player
        ret = pMediaEngineClassFactory->CreateInstance(MF_MEDIA_ENGINE_WAITFORSTABLE_STATE, pAttributes, &m_pMediaEngine);
        assert(ret == S_OK);

        // Release bullshits
        pAttributes->Release();
        pMediaEngineClassFactory->Release();
    }

    void MFPlayer::pause()
    {
        m_pMediaEngine->Pause();
        m_isPlaying = false;
    }

    void MFPlayer::play()
    {
        m_pMediaEngine->Play();
        m_isPlaying = true;
    }

    void MFPlayer::setLoop(bool bLoop)
    {
        m_pMediaEngine->SetLoop(bLoop ? TRUE : FALSE);
    }

    void MFPlayer::togglePlayPause()
    {
        if (m_pMediaEngine->IsPaused())
        {
            m_pMediaEngine->Play();
            m_isPlaying = true;
        }
        else
        {
            m_pMediaEngine->Pause();
            m_isPlaying = false;
        }
    }

    void MFPlayer::setPlayRate(double playRate)
    {
        m_pMediaEngine->SetPlaybackRate(playRate);
    }

    void MFPlayer::setSource(const std::string& url)
    {
        auto ws = utf8ToUtf16(url);
        BSTR bs = SysAllocStringLen(ws.data(), (UINT)ws.size());
        auto ret = m_pMediaEngine->SetSource(bs);
        assert(ret == S_OK);
    }

    void MFPlayer::setVolume(float volume)
    {
        if (m_pMediaEngine)
        {
            m_pMediaEngine->SetVolume(static_cast<double>(volume));
        }
    }

    bool MFPlayer::isPlaying() const
    {
        if (!m_pMediaEngine) return false;
        return m_isPlaying;
    }

    void MFPlayer::update()
    {
        if (!isPlaying()) return;
        if (m_pRenderTarget)
        {
            LONGLONG pts;
            if (m_pMediaEngine->OnVideoStreamTick(&pts) == S_OK)
            {
                MFVideoNormalizedRect videoRect;
                videoRect.left = 0;
                videoRect.top = 0;
                videoRect.right = 1;
                videoRect.bottom = 1;

                RECT targetRect;
                targetRect.left = 0;
                targetRect.top = 0;
                targetRect.right = static_cast<LONG>(m_pRenderTarget->getSize().x);
                targetRect.bottom = static_cast<LONG>(m_pRenderTarget->getSize().y);

                MFARGB borderColor;
                borderColor.rgbRed = 0;
                borderColor.rgbGreen = 0;
                borderColor.rgbBlue = 0;
                borderColor.rgbAlpha = 255;

                auto pRenderTargetD3D11 = ODynamicCast<OTextureD3D11>(m_pRenderTarget);
                m_pMediaEngine->TransferVideoFrame(pRenderTargetD3D11->getD3DTexture(), &videoRect, &targetRect, &borderColor);
            }
        }
    }

    const OTextureRef& MFPlayer::getRenderTarget()
    {
        return m_pRenderTarget;
    }
}

#endif
