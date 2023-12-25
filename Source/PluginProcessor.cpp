#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>

//------------------------------------------------------------------------------
BitcrusherAudioProcessor::BitcrusherAudioProcessor() :
    AudioProcessor
    (
        BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ),
    m_uiChanged(false),
    m_bitMask(0)
{
}

//------------------------------------------------------------------------------
bool BitcrusherAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // support mono or stereo output
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // check if the input layout matches the output layout
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true;
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;

    const int nInputChannels  = getTotalNumInputChannels();
    const int nSamples = buffer.getNumSamples();

    static constexpr float multiplier = 255.f;
    static constexpr float divisor = 1.f / multiplier;

    for (int iChan = 0; iChan < nInputChannels; ++iChan)
    {
        auto* pRead = buffer.getReadPointer(iChan);
        auto* pWrite = buffer.getWritePointer(iChan);

        for (int n = 0; n < nSamples; ++n)
        {
            const float inSample = pRead[n];

            float sign = 1.f;
            if (inSample < 0.f)
                sign = -1.f;

            const float inScaled = std::abs(std::round(inSample * multiplier));
            const uint8_t tmpResult = static_cast<uint8_t>(inScaled) & m_bitMask;
            const float outSample = static_cast<float>(tmpResult) * sign * divisor;

            pWrite[n] = outSample;
        }
    }
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
void BitcrusherAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
}

//------------------------------------------------------------------------------
juce::AudioProcessorEditor* BitcrusherAudioProcessor::createEditor()
{
    return new BitcrusherAudioProcessorEditor(*this);
}

//------------------------------------------------------------------------------
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BitcrusherAudioProcessor();
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::SetBitMask(uint8_t bitMask)
{
    m_bitMask = bitMask;
    m_uiChanged = true;
}
