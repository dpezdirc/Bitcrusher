//------------------------------------------------------------------------------
//  Code for the plugin's GUI
//------------------------------------------------------------------------------

#pragma once

#include <JuceHeader.h>
#include <array>

#include "PluginProcessor.h"

//------------------------------------------------------------------------------
class BitcrusherAudioProcessorEditor :
    public juce::AudioProcessorEditor
{
public:
    BitcrusherAudioProcessorEditor(BitcrusherAudioProcessor&);
    ~BitcrusherAudioProcessorEditor() override {};

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    static constexpr int N_BITS = 8;

    BitcrusherAudioProcessor& audioProcessor;
    std::array<juce::TextButton, N_BITS> m_buttons;
    juce::Label m_bitMaskLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitcrusherAudioProcessorEditor)
};
