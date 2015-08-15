//
//  ItemDefines.h
//  DungeonSmith
//
//  Created by The Drudgerist on 12/08/15.
//  Copyright (c) 2015 The Drudgerist. All rights reserved.
//

#ifndef DungeonSmith_ItemDefines_h
#define DungeonSmith_ItemDefines_h

namespace Entities {
    typedef enum {
        Item_None = 0,
        Item_Block,         // One voxel
        Item_Weapon_Sword,
        Item_Weapon_Knife,
        Item_Weapon_Axe,
        Item_Weapon_Lance,
        Item_Weapon_Bow,
        Item_Arrow_Wood,
        Item_Pickaxe,
        Item_Potion_Health,
        Item_Backpack,
        Item_Coin_1,
        Item_Coin_10,
        Item_Coin_100,
        Item_Torch,
        Item_Grenade,
        Item_Suckernade,
    } ItemType;
    
    inline static std::string NameForItem(const ItemType type) {
        std::string name;
        switch (type) {
            case Item_None:
                name = "UnknownItem";
                break;
            case Item_Block:
                name = "Block";
                break;
            case Item_Weapon_Sword:
                name = "Sword";
                break;
            case Item_Weapon_Knife:
                name = "Knife";
                break;
            case Item_Weapon_Axe:
                name = "Axe";
                break;
            case Item_Weapon_Lance:
                name = "Lance";
                break;
            case Item_Weapon_Bow:
                name = "Bow";
                break;
            case Item_Arrow_Wood:
                name = "Arrow";
                break;
            case Item_Pickaxe:
                name = "Pickaxe";
                break;
            case Item_Potion_Health:
                name = "Health Potion";
                break;
            case Item_Backpack:
                name = "Backpack";
                break;
            case Item_Coin_1:
                name = "1G";
                break;
            case Item_Coin_10:
                name = "10G";
                break;
            case Item_Coin_100:
                name = "100G";
                break;
            case Item_Torch:
                name = "Torch";
                break;
            case Item_Grenade:
                name = "Grenade";
                break;
            case Item_Suckernade:
                name = "Suckernade";
                break;
            default:
                break;
        }
        return name;
    }
}

#endif
