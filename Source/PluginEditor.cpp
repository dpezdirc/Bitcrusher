#include "PluginEditor.h"
#include "PluginProcessor.h"

#include <cassert>

const int MARGIN_WIDTH = 20;

//------------------------------------------------------------------------------
BitcrusherEditor::BitcrusherEditor(Processor& processor, juce::AudioProcessorValueTreeState& apvts) :
    AudioProcessorEditor(&processor),
    m_processor(processor)
{
    setSize (300, 200);

    //m_bitMaskLabel.setText("Bit Mask:", juce::dontSendNotification);
    //addAndMakeVisible(m_bitMaskLabel);

    for (int iBtn = 0; iBtn < Processor::N_BITS; ++iBtn)
    {
        m_buttons[iBtn].setButtonText(juce::String(iBtn + 1));
        m_buttons[iBtn].setClickingTogglesState(true);
        m_buttons[iBtn].addListener(this);

        juce::String buttonID = Processor::GetBitParamName(iBtn);
        m_buttons[iBtn].attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(apvts, buttonID, m_buttons[iBtn]));

        addAndMakeVisible(m_buttons[iBtn]);
    }
}

//------------------------------------------------------------------------------
void BitcrusherEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

//------------------------------------------------------------------------------
void BitcrusherEditor::resized()
{
    int xPos = MARGIN_WIDTH;
    int yPos = MARGIN_WIDTH;

    const int btnWidth = (getWidth() - (2 * MARGIN_WIDTH)) / Processor::N_BITS;

    for (juce::TextButton& btn : m_buttons)
    {
        btn.setBounds(xPos, yPos, btnWidth, btnWidth);
        xPos += btnWidth;
    }
}

//------------------------------------------------------------------------------
void BitcrusherEditor::buttonClicked(juce::Button* button)
{
    m_processor.OnUIChanged();
}
