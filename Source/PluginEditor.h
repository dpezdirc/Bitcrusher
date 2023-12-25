//------------------------------------------------------------------------------
//  Code for the plugin's GUI
//------------------------------------------------------------------------------

#pragma once

#include <JuceHeader.h>
#include <array>

#include "PluginProcessor.h"

//------------------------------------------------------------------------------
class BitcrusherAudioProcessorEditor :
    public juce::AudioProcessorEditor,
    public juce::ToggleButton::Listener
{
    using Processor = BitcrusherAudioProcessor;

    struct TextButtonWithAttachment :
        juce::TextButton
    {
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
    };

public:
    BitcrusherAudioProcessorEditor(Processor&, juce::AudioProcessorValueTreeState& apvts);
    ~BitcrusherAudioProcessorEditor() override {};

    void paint(juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button* button) override;

private:
    Processor& m_processor;
    std::array<TextButtonWithAttachment, Processor::N_BITS> m_buttons;
    juce::Label m_bitMaskLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitcrusherAudioProcessorEditor)
};
