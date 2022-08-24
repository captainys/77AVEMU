#pragma once

#include <string>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

#define ENABLE_EXAS_COMPILER

class ExasCompiler {
protected:
    size_t exasAddr;
    bool ready;
    std::vector<uint8_t> rom0;
    std::vector<uint8_t> rom12x;
public:
    ExasCompiler() : exasAddr(0), ready(false) {};

    void LoadROMFiles(std::string ROMPath) {
        {
            std::ifstream ifs(ROMPath + "EXAS-0.ROM", std::ios::binary);
            if (!ifs.is_open()) return;
            rom0.resize(64);
            ifs.read(reinterpret_cast<char*>(rom0.data()), 64);
        }

        std::vector<std::string> RomNames = { "EXAS-1.ROM", "EXAS-2.ROM", "EXAS-X.ROM" };
        constexpr size_t RomSize = 0x2000;
        rom12x.clear();
        for (auto it = RomNames.begin(); it != RomNames.end(); ++it) 
        {
            std::ifstream ifs(ROMPath + (*it), std::ios::binary);
            if (!ifs.is_open()) return;
            std::vector<uint8_t> buf;
            buf.resize(RomSize);
            ifs.read(reinterpret_cast<char*>(buf.data()), RomSize);
            rom12x.insert(rom12x.end(), buf.begin(), buf.end());
        }
        ready = true;
    }

    void IncrementAddress(void) {
        exasAddr ++;
        if (exasAddr >= rom12x.size()) 
        {
            exasAddr = 0;
        }
    }

    void Reset(void) {
        exasAddr = 0;
    }

    uint8_t ReadROM_12X(void) const {
        return rom12x[exasAddr];
    }

    uint8_t ReadROM_0(const size_t addr) const {
        return rom0[addr];
    }

    bool isReady(void) const {
        return ready;
    }

};
