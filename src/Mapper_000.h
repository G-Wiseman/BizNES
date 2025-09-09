#pragma once
#include <Mapper.h>


class Mapper_000 : public Mapper
{
public:
	Mapper_000(uint8_t num_PRG, uint8_t num_CHR);

public:
	size_t cpuReadMapped(uint16_t addr, bool& approved) override;
	size_t cpuWriteMapped(uint16_t addr, bool& approved) override;

	size_t ppuReadMapped(uint16_t addr, bool& approved) override;
	size_t ppuWriteMapped(uint16_t addr, bool & approved) override;
};



