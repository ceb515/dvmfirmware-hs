/**
* Digital Voice Modem - DSP Firmware (Hotspot)
* GPLv2 Open Source. Use is subject to license terms.
* DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
*
* @package DVM / DSP Firmware (Hotspot)
*
*/
//
// Based on code from the MMDVM_HS project. (https://github.com/juribeparada/MMDVM_HS)
// Licensed under the GPLv2 License (https://opensource.org/licenses/GPL-2.0)
//
/*
*   Copyright (C) 2016,2017 by Jonathan Naylor G4KLX
*   Copyright (C) 2016,2017 by Andy Uribe CA6JAU
*   Copyright (C) 2021 by Bryan Biedenkapp N2PLL
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#if !defined(__P25_TX_H__)
#define __P25_TX_H__

#include "Defines.h"
#include "SerialBuffer.h"

namespace p25
{
    // ---------------------------------------------------------------------------
    //  Constants
    // ---------------------------------------------------------------------------

    #define P25_FIXED_DELAY 300     // 300 = 62.49ms
                                    // Delay Value * 0.2083 = Preamble Length (ms)
    #define P25_FIXED_TAIL 600      // 600 = 500ms

    enum P25TXSTATE {
        P25TXSTATE_NORMAL,
        P25TXSTATE_CAL
    };

    // ---------------------------------------------------------------------------
    //  Class Declaration
    //      Implements transmitter logic for P25 mode operation.
    // ---------------------------------------------------------------------------

    class DSP_FW_API P25TX {
    public:
        /// <summary>Initializes a new instance of the P25TX class.</summary>
        P25TX();

        /// <summary>Process local buffer and transmit on the air interface.</summary>
        void process();

        /// <summary>Write data to the local buffer.</summary>
        uint8_t writeData(const uint8_t* data, uint8_t length);

        /// <summary>Clears the local buffer.</summary>
        void clear();

        /// <summary>Sets the FDMA preamble count.</summary>
        void setPreambleCount(uint8_t preambleCnt);
        /// <summary>Helper to set the calibration state for Tx.</summary>
        void setCal(bool start);

        /// <summary>Helper to get how much space the ring buffer has for samples.</summary>
        uint8_t getSpace() const;

    private:
        SerialBuffer m_fifo;

        P25TXSTATE m_state;

        uint8_t m_poBuffer[1200U];
        uint16_t m_poLen;
        uint16_t m_poPtr;

        uint16_t m_preambleCnt;
        uint16_t m_tailCnt;

        /// <summary></summary>
        void createData();
        /// <summary></summary>
        void createCal();

        /// <summary></summary>
        void writeByte(uint8_t c);
        /// <summary></summary>
        void writeSilence();
    };
} // namespace p25

#endif // __P25_TX_H__
