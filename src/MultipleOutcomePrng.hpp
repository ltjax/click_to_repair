#pragma once

class CMultipleOutcomePrng
{
public:
    CMultipleOutcomePrng(std::size_t N)
        : bias_(N, 1.f / N), dist_(0.f, 1.f)
    {
    }

    template <typename Rng>
    std::size_t operator()(Rng& rng)
    {
        auto N = bias_.size();
        // make the pick
        auto roll = dist_(rng);
        std::size_t pick = 0;
        float sum = 0.f;
        while (true)
        {
            sum += bias_[pick];

            if (roll < sum || pick+1 >= N)
                break;

            ++pick;
        }

        // rescale the bias
        auto increase = bias_[pick] / (N-1);
        for (std::size_t i = 0; i < bias_.size(); ++i)
        {
            bias_[i] = (i != pick) ? (bias_[i] + increase) : 0.f;             
        }

        return pick;
    }

private:
    std::vector<float> bias_;
    std::uniform_real_distribution<float> dist_;
};