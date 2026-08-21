#pragma once
#include <string>
#include <vector>

// WavLoader - reads a RIFF/WAVE file into mono float samples. Implemented after discovering oF 0.12.1 has no way of loading audio file samples
// Note: this class is pure C++ with no dependency on oF 
class WavLoader {
public:
    WavLoader();

    // Reads path, downmixes to mono, and resamples to targetRate
    // Returns false on any failure, in which case getSamples() is empty and getError() explains why
    bool load(const std::string& path, float targetRate);

    const std::vector<float>& getSamples() const { return samples; }
    const std::string& getError()   const { return error; }

    // What was in the file before downmixing and resampling
    int getSourceSampleRate() const { return sourceRate; }
    int getSourceChannels()   const { return sourceChannels; }

private:
    std::vector<float> samples;
    std::string        error;
    int                sourceRate;
    int                sourceChannels;

    void reset();
    bool readFile(const std::string& path, std::vector<unsigned char>& bytes);
    bool parse(const std::vector<unsigned char>& bytes);   // fills samples at sourceRate
    void resampleTo(float targetRate);
};
