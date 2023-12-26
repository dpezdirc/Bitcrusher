//------------------------------------------------------------------------------
//  Code for the plugin's audio processing logic
//------------------------------------------------------------------------------

#pragma once

#include <JuceHeader.h>

//------------------------------------------------------------------------------
class BitcrusherAudioProcessor :
    public juce::AudioProcessor
{
public:
    enum Operation
    {
        AND = 0,
        XOR
    };

public:
    BitcrusherAudioProcessor();
    ~BitcrusherAudioProcessor() override {};

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override {};
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void releaseResources() override {};

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    juce::AudioProcessorEditor* createEditor() override;

    //------------------------------------------
    // overridden, but otherwise unused methods
    //------------------------------------------

    bool hasEditor() const override { return true; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    const juce::String getName() const override { return JucePlugin_Name; }
    const juce::String getProgramName(int index) override { return {}; }
    void changeProgramName(int index, const juce::String& newName) override {}
    void setCurrentProgram(int index) override {}

    //------------------------------------------
    // custom, non-overridden methods
    //------------------------------------------

public:
    void OnUIChanged();

    static juce::String GetBitParamName(int iBit);

private:
    void UpdateParams();
    void UpdateBitMask();
    void UpdateOperation();

public:
    static constexpr int N_BITS = 8;
    static const juce::StringArray OPERATIONS;

private:
    std::atomic<bool> m_uiChanged;
    uint8_t m_bitMask;
    Operation m_operation;

    juce::AudioProcessorValueTreeState m_params;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BitcrusherAudioProcessor)
};
