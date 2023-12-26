#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <cmath>
#include <functional>
#include <chrono>

// copy of DBG, but works in release config
#define DBG_RELEASE(textToWrite) JUCE_BLOCK_WITH_FORCED_SEMICOLON (juce::String tempDbgBuf; tempDbgBuf << textToWrite; juce::Logger::outputDebugString (tempDbgBuf);)

const juce::StringArray BitcrusherAudioProcessor::OPERATIONS =
{
    "AND",
    "XOR"
};

//------------------------------------------------------------------------------
BitcrusherAudioProcessor::BitcrusherAudioProcessor() :
    AudioProcessor
    (
        BusesProperties().withInput("Input", juce::AudioChannelSet::stereo(), true)
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
    ),
    m_uiChanged(true),
    m_bitMask(0),
    m_operation(Operation::AND),
    m_params(*this, nullptr, juce::Identifier("BitcrusherParams"),
        {
            std::make_unique<juce::AudioParameterBool>(GetBitParamName(0), "Bit 1 state", false),
            std::make_unique<juce::AudioParameterBool>(GetBitParamName(1), "Bit 2 state", false),
            std::make_unique<juce::AudioParameterBool>(GetBitParamName(2), "Bit 3 state", false),
            std::make_unique<juce::AudioParameterBool>(GetBitParamName(3), "Bit 4 state", false),
            std::make_unique<juce::AudioParameterBool>(GetBitParamName(4), "Bit 5 state", false),
            std::make_unique<juce::AudioParameterBool>(GetBitParamName(5), "Bit 6 state", false),
            std::make_unique<juce::AudioParameterBool>(GetBitParamName(6), "Bit 7 state", false),
            std::make_unique<juce::AudioParameterBool>(GetBitParamName(7), "Bit 8 state", false),
            std::make_unique<juce::AudioParameterChoice>("operation", "Operation", OPERATIONS, static_cast<int>(m_operation))
        })
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

void BitcrusherAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    juce::AudioBuffer<float> buffer(2, 480);
    juce::MidiBuffer midi;

    auto start = std::chrono::system_clock::now();

    const int nIterations = 100000;
    for (int i = 0; i < nIterations; i++)
    {
        processBlock(buffer, midi);
    }

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    double elapsedAvg = (double)elapsed.count() / nIterations;

    DBG_RELEASE("Elapsed time: " << elapsed.count() << " ms over " << nIterations << " iterations (" << elapsedAvg << " ms per iteration)");
}

//------------------------------------------------------------------------------
uint8_t BitwiseNand(uint8_t in, uint8_t mask)
{
    return in & ~mask;
}

//------------------------------------------------------------------------------
uint8_t BitwiseXor(uint8_t in, uint8_t mask)
{
    // zero the first 3 bits of the mask to avoid excessive noise
    // TODO: gain reduction, then this may not be needed
    mask = mask & 31;

    return in ^ mask;
}

//------------------------------------------------------------------------------
std::function<uint8_t(uint8_t, uint8_t)> GetOperationFunc(BitcrusherAudioProcessor::Operation operationType)
{
    std::function<uint8_t(uint8_t, uint8_t)> func;

    switch (operationType)
    {
        case BitcrusherAudioProcessor::Operation::AND:
        {
            func = BitwiseNand;
            break;
        }
        case BitcrusherAudioProcessor::Operation::XOR:
        {
            func = BitwiseXor;
            break;
        }
    }

    return func;
}

//------------------------------------------------------------------------------
auto GetOperationFunc_PlainPtr(BitcrusherAudioProcessor::Operation operationType)
{
    switch (operationType)
    {
        case BitcrusherAudioProcessor::Operation::AND:
        {
            return BitwiseNand;
        }
        case BitcrusherAudioProcessor::Operation::XOR:
        {
            return BitwiseXor;
        }
    }
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    if (m_uiChanged)
    {
        UpdateParams();
        m_uiChanged = false;
    }

    // the operation to perform
    //std::function<uint8_t(uint8_t, uint8_t)> OperationFunc = GetOperationFunc(m_operation);
    auto OperationFunc = GetOperationFunc_PlainPtr(m_operation);

    const int nInputChannels = getTotalNumInputChannels();
    const int nSamples = buffer.getNumSamples();

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

            static constexpr float multiplier = 255.f;
            static constexpr float divisor = 1.f / multiplier;

            const float inScaled = std::abs(std::round(inSample * multiplier));
            const uint8_t tmpResult = OperationFunc(static_cast<uint8_t>(inScaled), m_bitMask);

            //const uint8_t tmpResult = m_operation == Operation::AND ?
            //    BitwiseNand(static_cast<uint8_t>(inScaled), m_bitMask) :
            //    BitwiseXor(static_cast<uint8_t>(inScaled), m_bitMask);

            const float outSample = static_cast<float>(tmpResult) * sign * divisor;

            pWrite[n] = outSample;
        }
    }
}

//------------------------------------------------------------------------------
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
    return new BitcrusherEditor(*this, m_params);
}

//------------------------------------------------------------------------------
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BitcrusherAudioProcessor();
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::OnUIChanged()
{
    m_uiChanged = true;
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::UpdateParams()
{
    UpdateBitMask();
    UpdateOperation();
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::UpdateBitMask()
{
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

    m_bitMask = 0; // 00000000

    // set the bit mask based on the state of each button
    for (int iBit = 0; iBit < 8; iBit++)
    {
        juce::RangedAudioParameter* pParam = m_params.getParameter(GetBitParamName(iBit));
        const bool bitState = static_cast<juce::AudioParameterBool*>(pParam)->get();
        if (bitState)
            m_bitMask |= BIT_OPERANDS[iBit];
    }
}

//------------------------------------------------------------------------------
void BitcrusherAudioProcessor::UpdateOperation()
{
    juce::RangedAudioParameter* pParam = m_params.getParameter("operation");
    const int selectedIndex = static_cast<juce::AudioParameterChoice*>(pParam)->getIndex();
    m_operation = static_cast<Operation>(selectedIndex);
}

//------------------------------------------------------------------------------
juce::String BitcrusherAudioProcessor::GetBitParamName(int iBit)
{
    return juce::String("bit") + juce::String(iBit + 1);
}
