#include "PluginEditor.h"
#include "PluginProcessor.h"

const int MARGIN_WIDTH = 20;

//------------------------------------------------------------------------------
BitcrusherAudioProcessorEditor::BitcrusherAudioProcessorEditor(BitcrusherAudioProcessor& p) :
    AudioProcessorEditor(&p),
    audioProcessor(p)
{
    setSize (300, 200);

    //m_bitMaskLabel.setText("Bit Mask:", juce::dontSendNotification);
    //addAndMakeVisible(m_bitMaskLabel);

    for (int iBtn = 0; iBtn < N_BITS; iBtn++)
    {
        m_buttons[iBtn].setButtonText(juce::String(iBtn));
        m_buttons[iBtn].setClickingTogglesState(true);
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
