#include "PluginEditor.h"
#include "PluginProcessor.h"

const int MARGIN_WIDTH = 20;

//------------------------------------------------------------------------------
BitcrusherEditor::BitcrusherEditor(Processor& processor, juce::AudioProcessorValueTreeState& apvts) :
    AudioProcessorEditor(&processor),
    m_processor(processor)
{
    // set up buttons

    for (int iBtn = 0; iBtn < Processor::N_BITS; ++iBtn)
    {
        m_buttons[iBtn].setButtonText(juce::String(iBtn + 1));
        m_buttons[iBtn].setClickingTogglesState(true);
        m_buttons[iBtn].addListener(this);

        juce::String buttonID = Processor::GetBitParamName(iBtn);
        m_buttons[iBtn].attachment.reset(new juce::AudioProcessorValueTreeState::ButtonAttachment(apvts, buttonID, m_buttons[iBtn]));

        addAndMakeVisible(m_buttons[iBtn]);
    }

    // set up combo box and label

    for (int i = 0; i < Processor::OPERATIONS.size(); ++i)
    {
        m_operationsComboBox.addItem(Processor::OPERATIONS[i], i + 1);
    }
    m_operationsComboBox.addListener(this);
    m_operationsComboBox.attachment.reset(new juce::AudioProcessorValueTreeState::ComboBoxAttachment(apvts, "operation", m_operationsComboBox));
    addAndMakeVisible(m_operationsComboBox);

    m_operationsLabel.setText("Operation: ", juce::dontSendNotification);
    m_operationsLabel.attachToComponent(&m_operationsComboBox, true);

    setSize(300, 150);
}

//------------------------------------------------------------------------------
void BitcrusherEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

//------------------------------------------------------------------------------
int GetLabelWidth(juce::Label& label)
{
    const int textWidth = label.getFont().getStringWidth(label.getText());
    const int borderWidth = label.getBorderSize().getLeftAndRight();

    return textWidth + borderWidth;
}

//------------------------------------------------------------------------------
void BitcrusherEditor::resized()
{
    int xPos = MARGIN_WIDTH;
    int yPos = MARGIN_WIDTH;

    // buttons

    const int btnWidth = (getWidth() - (2 * MARGIN_WIDTH)) / Processor::N_BITS;
    
    for (juce::TextButton& btn : m_buttons)
    {
        btn.setBounds(xPos, yPos, btnWidth, btnWidth);
        xPos += btnWidth;
    }
    
    xPos = MARGIN_WIDTH;
    yPos = MARGIN_WIDTH * 2 + btnWidth;

    // combo box and label

    const int labelWidth = GetLabelWidth(m_operationsLabel);
    m_operationsComboBox.setBounds(xPos + labelWidth, yPos, (getWidth() - 2 * MARGIN_WIDTH) * 5 / 8 - labelWidth, 20);
}

//------------------------------------------------------------------------------
void BitcrusherEditor::buttonClicked(juce::Button*)
{
    m_processor.OnUIChanged();
}

//------------------------------------------------------------------------------
void BitcrusherEditor::comboBoxChanged(juce::ComboBox*)
{
    m_processor.OnUIChanged();
}
