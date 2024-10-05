#pragma once
#include <map>
#include <string>

static std::map<std::string, std::string> EntityType = {
    {"chicken", "Chicken"},
    {"hostage_entity", "Hostage"}
};

static std::map<std::string, std::string> ProjectilesType = {
    {"smokegrenade_projectile", "Smoke Grenade"},
    {"flashbang_projectile", "Flashbang"},
    {"hegrenade_projectile", "HE Grenade"},
    {"molotov_projectile", "Molotov"},
    {"incendiarygrenade_projectile", "Incendiary Grenade"},
    {"decoy_projectile", "Decoy Grenade"}
};

static std::map<std::string, std::string> WeaponsType = {
    {"weapon_ak47", "AK-47"},
    {"weapon_m4a1", "M4A1"},
    {"weapon_awp", "AWP"},
    {"weapon_elite", "Elite"},
    {"weapon_famas", "Famas"},
    {"weapon_flashbang", "Flashbang"},
    {"weapon_g3sg1", "G3SG1"},
    {"weapon_galilar", "Galil AR"},
    {"weapon_healthshot", "Health Shot"},
    {"weapon_hegrenade", "HE Grenade"},
    {"weapon_incgrenade", "Incendiary Grenade"},
    {"weapon_m249", "M249"},
    {"weapon_m4a1_silencer", "M4A1-S"},
    {"weapon_mac10", "MAC-10"},
    {"weapon_mag7", "MAG-7"},
    {"weapon_molotov", "Molotov"},
    {"weapon_mp5sd", "MP5-SD"},
    {"weapon_mp7", "MP7"},
    {"weapon_mp9", "MP9"},
    {"weapon_negev", "Negev"},
    {"weapon_nova", "Nova"},
    {"weapon_p90", "P90"},
    {"weapon_sawedoff", "Sawed-Off"},
    {"weapon_scar20", "SCAR-20"},
    {"weapon_sg556", "SG 553"},
    {"weapon_smokegrenade", "Smoke Grenade"},
    {"weapon_ssg08", "SSG 08"},
    {"weapon_tagrenade", "TA Grenade"},
    {"weapon_taser", "Taser"},
    {"weapon_ump45", "UMP-45"},
    {"weapon_xm1014", "XM1014"},
    {"weapon_aug", "AUG"},
    {"weapon_bizon", "PP-Bizon"},
    {"weapon_decoy", "Decoy Grenade"},
    {"weapon_fiveseven", "Five-Seven"},
    {"weapon_hkp2000", "P2000"},
    {"weapon_usp_silencer", "USP-S"},
    {"weapon_p250", "P250"},
    {"weapon_tec9", "Tec-9"},
    {"weapon_cz75a", "CZ75-Auto"},
    {"weapon_deagle", "Desert Eagle"},
    {"weapon_revolver", "R8 Revolver"},
    {"weapon_glock", "Glock-18"}
};

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
    {31, "zeus"},  // Taser
    {32, "p2000"},
    {33, "mp7"},
    {34, "mp9"},
    {35, "nova"},
    {36, "p250"},
    {37, "shield"},  // Shield (if applicable)
    {38, "scar20"},
    {39, "sg556"},
    {40, "ssg08"},
    {41, "knife_egg"},  // Knife Egg (if applicable)
    {42, "ct_knife"},
    {43, "flashbang"},
    {44, "hegrenade"},
    {45, "smokegrenade"},
    {46, "molotov"},
    {47, "decoy"},
    {48, "incgrenade"},
    {49, "c4"},
    {57, "healthshot"},
    {59, "t_knife"},
    {60, "m4a1_silencer"},
    {61, "usp_silencer"},
    {63, "cz75a"},
    {64, "revolver"},
    {68, "tagrenade"},  // Tactical Grenade (if applicable)
    {69, "fists"},
    {70, "breachcharge"},
    {72, "tablet"},
    {74, "melee"},
    {75, "axe"},
    {76, "hammer"},
    {78, "spanner"},  // Spanner (if applicable)
    {80, "knife_ghost"},
    {81, "firebomb"},
    {82, "diversion"},  // Diversion (if applicable)
    {83, "frag_grenade"},
    {84, "snowball"},
    {85, "bumpmine"},
    {500, "bayonet"},
    {503, "knife_css"},
    {505, "knife_flip"},
    {506, "knife_gut"},
    {507, "knife_karambit"},
    {508, "knife_m9_bayonet"},
    {509, "knife_tactical"},
    {512, "knife_falchion"},
    {514, "knife_survival_bowie"},
    {515, "knife_butterfly"},
    {516, "knife_push"},
    {517, "knife_cord"},
    {518, "knife_canis"},
    {519, "knife_ursus"},
    {520, "knife_gypsy_jackknife"},
    {521, "knife_outdoor"},
    {522, "knife_stiletto"},
    {523, "knife_widowmaker"},
    {525, "knife_skeleton"},
    {526, "knife_kukri"},
    {5027, "studded_bloodhound_gloves"},
    {5028, "t_gloves"},
    {5029, "ct_gloves"},
    {5030, "sporty_gloves"},
    {5031, "slick_gloves"},
    {5032, "leather_handwraps"},
    {5033, "motorcycle_gloves"},
    {5034, "specialist_gloves"},
    {5035, "studded_hydra_gloves"}
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