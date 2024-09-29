#pragma once
#include <map>
#include <string>

static std::map<int, std::string> Weapon = {
    {1, "deagle"},
    {2, "elite"},
    {3, "fiveseven"},
    {4, "glock"},
    {7, "ak47"},
    {8, "aug"},
    {9, "awp"},
    {10, "famas"},
    {11, "g3sg1"},
    {13, "galilar"},
    {14, "m249"},
    {16, "m4a1"},
    {17, "mac10"},
    {19, "p90"},
    {23, "mp5sd"},
    {24, "ump45"},
    {25, "xm1014"},
    {26, "bizon"},
    {27, "mag7"},
    {28, "negev"},
    {29, "sawedoff"},
    {30, "tec9"},
    {31, "zeus"},
    {32, "p2000"},
    {33, "mp7"},
    {34, "mp9"},
    {35, "nova"},
    {36, "p250"},
    {38, "scar20"},
    {39, "sg556"},
    {40, "ssg08"},
    {42, "ct_knife"},
    {43, "flashbang"},
    {44, "hegrenade"},
    {45, "smokegrenade"},
    {46, "molotov"},
    {47, "decoy"},
    {48, "incgrenade"},
    {49, "c4"},
    {59, "t_knife"},
    {60, "m4a1_silencer"},
    {61, "usp"},
    {63, "cz75a"},
    {64, "revolver"},
    {500, "bayonet"},
    {505, "knife_flip"},
    {506, "knife_gut"},
    {507, "knife_karambit"},
    {508, "knife_m9_bayonet"},
    {509, "knife_tactical"},
    {512, "knife_falchion"},
    {514, "knife_survival_bowie"},
    {515, "knife_butterfly"},
    {516, "knife_push"}
};

static std::map<int, std::string> gunIcons = {
    {7, "W"},   // ak47
    {16, "M"},  // m4a1
    {34, "R"},  // mp9
    {10, "H"},  // famas
    {24, "b"},  // ump45
    {4, "D"},   // glock
    {42, "]"},  // knife_ct
    {59, "["},  // knife_t
    {1, "A"},   // deagle
    {2, "B"},   // elite
    {3, "C"},   // fiveseven
    {64, "J"},  // revolver
    {32, "E"},  // hkp2000
    {36, "F"},  // p250
    {61, "G"},  // usp_silencer
    {30, "H"},  // tec9
    {63, "I"},  // cz75a
    {17, "K"},  // mac10
    {26, "M"},  // bizon
    {33, "N"},  // mp7
    {19, "P"},  // p90
    {13, "Q"},  // galilar
    {60, "T"},  // m4a1_silencer
    {8, "U"},   // aug
    {39, "V"},  // sg556
    {7, "W"},   // ak47 (again for consistency)
    {11, "X"},  // g3sg1
    {38, "Y"},  // scar20
    {9, "Z"},   // awp
    {40, "a"},  // ssg08
    {40, "a"},  // ssg-08 (duplicate for consistency)
    {25, "b"},  // xm1014
    {29, "c"},  // sawedoff
    {27, "d"},  // mag7
    {35, "e"},  // nova
    {28, "f"},  // negev
    {14, "g"},  // m249
    {31, "h"},  // taser
    {43, "i"},  // flashbang
    {44, "j"},  // hegrenade
    {45, "k"},  // smokegrenade
    {46, "l"},  // molotov
    {47, "m"},  // decoy
    {48, "n"},  // incgrenade
    {49, "o"},  // c4
};