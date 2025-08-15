#ifndef VALUENOISE2D_HPP
#define VALUENOISE2D_HPP

#include <cstdint>
#include <cmath>
#include <algorithm>
#include <vector>
#include <type_traits>

/// ValueNoise2D
///  - Deterministic, seedable 2D value noise
///  - Optional tiling (periodic in X/Y)
///  - Switchable fade curve (cubic/quintic)
///  - fBm / turbulence / ridge fractals (normalized)
///  - Optional derivatives d/dx, d/dy for shading
///  - Domain warping helper
///  - Convenience grid fill
///
/// Ranges:
///  - Base noise:      [-1, 1]
///  - noise01():       [ 0, 1]
///  - fBmNormalized(): [-1, 1] (normalized by total amplitude)
///
/// Note: All functions are thread-safe after construction (read-only state).

class ValueNoise2D {
public:
    /// Fade curve selection: CUBIC = t^2(3-2t), QUINTIC = 6t^5 - 15t^4 + 10t^3
    enum class Fade : std::uint8_t { Cubic, Quintic };

    explicit ValueNoise2D(std::uint32_t seed = 0x12345678u, Fade fade = Fade::Quintic) noexcept
        : _seed(seed), _fade(fade) {}

    void setSeed(std::uint32_t s) noexcept { _seed = s; }
    std::uint32_t seed() const noexcept { return _seed; }

    void setFade(Fade f) noexcept { _fade = f; }
    Fade fade() const noexcept { return _fade; }

    /// Single sample of value noise at (x, y).
    /// Optional tiling if periodX/periodY > 0.
    /// Returns value in [-1, 1].
    float noise(float x, float y, int periodX = 0, int periodY = 0) const noexcept {
        int xi0, yi0;
        float xf, yf;
        latticeCoords_(x, y, xi0, yi0, xf, yf);

        const int xi1 = xi0 + 1;
        const int yi1 = yi0 + 1;

        const int X00 = wrap_(xi0, periodX);
        const int X10 = wrap_(xi1, periodX);
        const int X01 = X00;
        const int X11 = X10;
        const int Y00 = wrap_(yi0, periodY);
        const int Y10 = Y00;
        const int Y01 = wrap_(yi1, periodY);
        const int Y11 = Y01;

        const float v00 = latticeValue_(X00, Y00);
        const float v10 = latticeValue_(X10, Y10);
        const float v01 = latticeValue_(X01, Y01);
        const float v11 = latticeValue_(X11, Y11);

        const float u = fade_(xf);
        const float v = fade_(yf);

        // Bilinear interpolation with smooth fade
        const float x0 = lerp_(v00, v10, u);
        const float x1 = lerp_(v01, v11, u);
        return lerp_(x0, x1, v);
    }

    /// Variant with derivatives (d/dx, d/dy) if out params are non-null.
    /// Derivatives are of the *interpolant*, scaled by chosen fade curve derivative.
    float noiseWithDerivatives(
        float x, float y,
        float* dnx /*out*/ = nullptr, float* dny /*out*/ = nullptr,
        int periodX = 0, int periodY = 0
    ) const noexcept {
        int xi0, yi0;
        float xf, yf;
        latticeCoords_(x, y, xi0, yi0, xf, yf);
        const int xi1 = xi0 + 1;
        const int yi1 = yi0 + 1;

        const int X00 = wrap_(xi0, periodX);
        const int X10 = wrap_(xi1, periodX);
        const int X01 = X00;
        const int X11 = X10;
        const int Y00 = wrap_(yi0, periodY);
        const int Y10 = Y00;
        const int Y01 = wrap_(yi1, periodY);
        const int Y11 = Y01;

        const float v00 = latticeValue_(X00, Y00);
        const float v10 = latticeValue_(X10, Y10);
        const float v01 = latticeValue_(X01, Y01);
        const float v11 = latticeValue_(X11, Y11);

        const float u  = fade_(xf);
        const float v  = fade_(yf);
        const float du = fadeDeriv_(xf);
        const float dv = fadeDeriv_(yf);

        // Bilinear blend: f(u,v) = (1-u)(1-v) v00 + u(1-v) v10 + (1-u)v v01 + uv v11
        // d/du = (1-v)(v10 - v00) + v(v11 - v01)
        // d/dv = (1-u)(v01 - v00) + u(v11 - v10)
        const float dudf = (1.0f - v) * (v10 - v00) + v * (v11 - v01);
        const float dvdf = (1.0f - u) * (v01 - v00) + u * (v11 - v10);

        if (dnx) *dnx = dudf * du;  // chain rule with fade'(xf)
        if (dny) *dny = dvdf * dv;  // chain rule with fade'(yf)

        // Value
        const float x0 = lerp_(v00, v10, u);
        const float x1 = lerp_(v01, v11, u);
        return lerp_(x0, x1, v);
    }

    /// Map noise into [0, 1].
    float noise01(float x, float y, int periodX = 0, int periodY = 0) const noexcept {
        return 0.5f * (noise(x, y, periodX, periodY) + 1.0f);
    }

    // ---------------------------------------------------------------------
    // Fractal combinations (normalized)
    // ---------------------------------------------------------------------

    /// Fractional Brownian motion (fBm). Returns roughly in [-1, 1] when normalized=true.
    float fbm(
        float x, float y,
        int octaves = 5,
        float lacunarity = 2.0f,
        float gain = 0.5f,
        int periodX = 0, int periodY = 0,
        bool normalized = true
    ) const noexcept {
        float sum = 0.0f;
        float amp = 1.0f;
        float freq = 1.0f;
        float maxAmp = 0.0f;

        for (int i = 0; i < octaves; ++i) {
            sum    += amp * noise(x * freq, y * freq, periodScale_(periodX, freq), periodScale_(periodY, freq));
            maxAmp += amp;
            amp    *= gain;
            freq   *= lacunarity;
        }
        return normalized && maxAmp > 0.0f ? sum / maxAmp : sum;
    }

    /// Turbulence: sum of |noise|. Returns ~[0,1] when normalized=true.
    float turbulence(
        float x, float y,
        int octaves = 5,
        float lacunarity = 2.0f,
        float gain = 0.5f,
        int periodX = 0, int periodY = 0,
        bool normalized = true
    ) const noexcept {
        float sum = 0.0f;
        float amp = 1.0f;
        float freq = 1.0f;
        float maxAmp = 0.0f;

        for (int i = 0; i < octaves; ++i) {
            sum    += amp * std::fabs(noise(x * freq, y * freq, periodScale_(periodX, freq), periodScale_(periodY, freq)));
            maxAmp += amp;
            amp    *= gain;
            freq   *= lacunarity;
        }
        return normalized && maxAmp > 0.0f ? sum / maxAmp : sum;
    }

    /// Ridge: 1 - |noise| then summed; returns ~[0,1] when normalized=true.
    float ridge(
        float x, float y,
        int octaves = 5,
        float lacunarity = 2.0f,
        float gain = 0.5f,
        int periodX = 0, int periodY = 0,
        bool normalized = true
    ) const noexcept {
        float sum = 0.0f;
        float amp = 1.0f;
        float freq = 1.0f;
        float maxAmp = 0.0f;

        for (int i = 0; i < octaves; ++i) {
            float n = 1.0f - std::fabs(noise(x * freq, y * freq, periodScale_(periodX, freq), periodScale_(periodY, freq)));
            sum    += amp * n;
            maxAmp += amp;
            amp    *= gain;
            freq   *= lacunarity;
        }
        return normalized && maxAmp > 0.0f ? sum / maxAmp : sum;
    }

    // ---------------------------------------------------------------------
    // Domain warping helper
    // ---------------------------------------------------------------------

    /// Simple domain warp: displaces (x,y) using fBm fields; useful to break up grid artifacts.
    /// Modifies x,y in-place.
    void domainWarp(
        float& x, float& y,
        float amplitude = 20.0f,
        float frequency = 1.0f,
        int octaves = 3,
        float lacunarity = 2.0f,
        float gain = 0.5f,
        int periodX = 0, int periodY = 0
    ) const noexcept {
        // Offsets break correlation between the two warp fields
        const float wx = fbm(x * frequency + 37.0f, y * frequency + 17.0f,
                             octaves, lacunarity, gain,
                             periodScale_(periodX, frequency), periodScale_(periodY, frequency), true);
        const float wy = fbm(x * frequency - 53.0f, y * frequency - 29.0f,
                             octaves, lacunarity, gain,
                             periodScale_(periodX, frequency), periodScale_(periodY, frequency), true);
        x += wx * amplitude;
        y += wy * amplitude;
    }

    // ---------------------------------------------------------------------
    // Grid fill convenience
    // ---------------------------------------------------------------------

    /// Fills a flat buffer with (optionally) tiled fBm (or base noise if octaves==1).
    /// out must have at least width*height floats.
    void fill2D(
        float* out,
        int width, int height,
        float scaleX, float scaleY,
        int octaves = 1, float lacunarity = 2.0f, float gain = 0.5f,
        int periodX = 0, int periodY = 0,
        bool normalized = true
    ) const noexcept {
        if (!out || width <= 0 || height <= 0) return;
        for (int y = 0; y < height; ++y) {
            const float fy = static_cast<float>(y) * scaleY;
            for (int x = 0; x < width; ++x) {
                const float fx = static_cast<float>(x) * scaleX;
                float v;
                if (octaves <= 1) {
                    v = noise(fx, fy, periodX, periodY);
                } else {
                    v = fbm(fx, fy, octaves, lacunarity, gain, periodX, periodY, normalized);
                }
                out[y * width + x] = v;
            }
        }
    }

private:
    // ---------------- Internals ----------------

    std::uint32_t _seed;
    Fade _fade;

    // Positive modulo wrap for tiling
    static inline int wrap_(int i, int period) noexcept {
        if (period <= 0) return i;
        int r = i % period;
        return (r < 0) ? (r + period) : r;
    }

    // If a period is given and frequency != 1, scale it so the tiling remains consistent.
    static inline int periodScale_(int period, float freq) noexcept {
        if (period <= 0) return 0;
        // Round to keep integer lattice periodicity coherent at scaled frequency
        const int p = static_cast<int>(std::round(static_cast<float>(period) * freq));
        return std::max(1, p);
    }

    static inline float lerp_(float a, float b, float t) noexcept {
        return a + t * (b - a);
    }

    static inline float fadeCubic_(float t) noexcept {
        // t^2 * (3 - 2 t)
        return t * t * (3.0f - 2.0f * t);
    }
    static inline float fadeCubicDeriv_(float t) noexcept {
        // derivative: 6 t (1 - t)
        return 6.0f * t * (1.0f - t);
    }
    static inline float fadeQuintic_(float t) noexcept {
        // 6 t^5 - 15 t^4 + 10 t^3
        return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
    }
    static inline float fadeQuinticDeriv_(float t) noexcept {
        // derivative: 30 t^2 (t - 1)^2
        const float t2 = t * t;
        return 30.0f * t2 * (t - 1.0f) * (t - 1.0f);
    }

    inline float fade_(float t) const noexcept {
        return (_fade == Fade::Quintic) ? fadeQuintic_(t) : fadeCubic_(t);
    }
    inline float fadeDeriv_(float t) const noexcept {
        return (_fade == Fade::Quintic) ? fadeQuinticDeriv_(t) : fadeCubicDeriv_(t);
    }

    static inline void latticeCoords_(float x, float y, int& xi, int& yi, float& xf, float& yf) noexcept {
        // floorf handles negatives correctly
        xi = static_cast<int>(std::floor(x));
        yi = static_cast<int>(std::floor(y));
        xf = x - static_cast<float>(xi);
        yf = y - static_cast<float>(yi);
        // Clamp fractional to [0,1] for numeric robustness
        xf = std::max(0.0f, std::min(1.0f, xf));
        yf = std::max(0.0f, std::min(1.0f, yf));
    }

    // A good quality 2D hash -> float in [-1, 1].
    inline float latticeValue_(int x, int y) const noexcept {
        std::uint32_t h = hash2_(static_cast<std::uint32_t>(x), static_cast<std::uint32_t>(y), _seed);
        // Map [0, 2^32-1] -> [0,1] then -> [-1,1]
        constexpr float inv = 1.0f / 4294967295.0f;
        return (static_cast<float>(h) * inv) * 2.0f - 1.0f;
    }

    // SplitMix/Jenkins-style avalanche for stable, fast hashing of (x,y,seed)
    static inline std::uint32_t hash2_(std::uint32_t x, std::uint32_t y, std::uint32_t seed) noexcept {
        std::uint32_t h = x * 0x9E3779B1u ^ y * 0x85EBCA77u ^ seed * 0xC2B2AE3Du;
        h ^= h >> 16; h *= 0x7feb352du;
        h ^= h >> 15; h *= 0x846ca68bu;
        h ^= h >> 16;
        return h;
    }
};

#endif // VALUENOISE2D_HPP
