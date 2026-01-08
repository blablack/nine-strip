#pragma once

class DCBlocker
{
   public:
    DCBlocker();
    ~DCBlocker();

    void prepare(double sampleRate);
    void reset();

    template <typename SampleType>
    void processStereo(SampleType** channels, int numSamples);

   private:
    double alpha;
    double x_prevL, y_prevL;
    double x_prevR, y_prevR;
};