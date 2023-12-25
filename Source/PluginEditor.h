//------------------------------------------------------------------------------
//  Code for the plugin's GUI
//------------------------------------------------------------------------------

#pragma once

#include <JuceHeader.h>
#include <array>
#include <memory>

#include "PluginProcessor.h"

//------------------------------------------------------------------------------
class BitcrusherEditor :
    public juce::AudioProcessorEditor,
    public juce::ToggleButton::Listener,
    public juce::ComboBox::Listener
{
    //------------------------------------------------------------------------------
    struct TextButtonWithAttachment : juce::TextButton
    {
        std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attachment;
    };

    //------------------------------------------------------------------------------
    struct ComboBoxWithAttachment : juce::ComboBox
    {
        std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> attachment;
    };

    using Processor = BitcrusherAudioProcessor;

public:
    BitcrusherEditor(Processor&, juce::AudioProcessorValueTreeState&);
    ~BitcrusherEditor() override {};

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button*) override;
    void comboBoxChanged(juce::ComboBox*) override;

private:
    Processor& m_processor;
    std::array<TextButtonWithAttachment, Processor::N_BITS> m_buttons;
    ComboBoxWithAttachment m_operationsComboBox;
    juce::Label m_operationsLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitcrusherEditor)
};
