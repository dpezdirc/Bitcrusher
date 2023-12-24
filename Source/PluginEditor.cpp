#include "PluginEditor.h"
#include "PluginProcessor.h"

#include <cassert>

const int MARGIN_WIDTH = 20;

//------------------------------------------------------------------------------
BitcrusherAudioProcessorEditor::BitcrusherAudioProcessorEditor(BitcrusherAudioProcessor& p) :
    AudioProcessorEditor(&p),
    m_processor(p)
{
    setSize (300, 200);

    //m_bitMaskLabel.setText("Bit Mask:", juce::dontSendNotification);
    //addAndMakeVisible(m_bitMaskLabel);

    for (int iBtn = 0; iBtn < N_BITS; iBtn++)
    {
        m_buttons[iBtn].setButtonText(juce::String(iBtn + 1));
        m_buttons[iBtn].setClickingTogglesState(true);
        m_buttons[iBtn].addListener(this);
        addAndMakeVisible(m_buttons[iBtn]);
    }
    
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessorEditor::resized()
{
    int xPos = MARGIN_WIDTH;
    int yPos = MARGIN_WIDTH;

    const int btnWidth = (getWidth() - (2 * MARGIN_WIDTH)) / N_BITS;

    for (juce::TextButton& btn : m_buttons)
    {
        btn.setBounds(xPos, yPos, btnWidth, btnWidth);
        xPos += btnWidth;
    }
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    // maps a button's index to its corresponding bit mask value 
    static constexpr uint8_t BIT_OPERANDS[8] =
    {
        0x80, // 10000000
        0x40, // 01000000
        0x20, // 00100000
        0x10, // 00010000
        0x08, // 00001000
        0x04, // 00000100
        0x02, // 00000010
        0x01  // 00000001
    };

    uint8_t bitMask = 0; // 00000000

    // set the bit mask based on the state of each button
    for (int iBtn = 0; iBtn < m_buttons.size(); iBtn++)
    {
        if (m_buttons[iBtn].getToggleState())
            bitMask |= BIT_OPERANDS[iBtn];
    }

    m_processor.SetBitMask(bitMask);
}

//------------------------------------------------------------------------------
int BitcrusherAudioProcessorEditor::GetButtonIndex(juce::Button* button)
{
    int iBtn = 0;

    while (iBtn < m_buttons.size())
    {
        if (button == &m_buttons[iBtn])
            break;

        ++iBtn;
    }

    assert(iBtn < m_buttons.size());

    return iBtn;
}
