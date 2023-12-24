#include "PluginProcessor.h"
#include "PluginEditor.h"

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
void BitcrusherAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto nInputChannels  = getTotalNumInputChannels();
    auto nOutputChannels = getTotalNumOutputChannels();

    for (int iChan = 0; iChan < nInputChannels; ++iChan)
    {
        auto* channelData = buffer.getWritePointer (iChan);

        // ..do something to the data...
    }
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
