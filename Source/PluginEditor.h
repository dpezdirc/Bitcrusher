//------------------------------------------------------------------------------
//  Code for the plugin's GUI
//------------------------------------------------------------------------------

#pragma once

#include <JuceHeader.h>
#include <array>

#include "PluginProcessor.h"

//------------------------------------------------------------------------------
class BitcrusherEditor :
    public juce::AudioProcessorEditor,
    public juce::ToggleButton::Listener
{
    //------------------------------------------------------------------------------
    struct TextButtonWithAttachment :
        juce::TextButton
    {
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
    };

    using Processor = BitcrusherAudioProcessor;

public:
    BitcrusherEditor(Processor&, juce::AudioProcessorValueTreeState&);
    ~BitcrusherEditor() override {};

    void paint(juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button*) override;

private:
    Processor& m_processor;
    std::array<TextButtonWithAttachment, Processor::N_BITS> m_buttons;
    juce::Label m_bitMaskLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitcrusherEditor)
};
