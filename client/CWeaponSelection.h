/*****************************************************************************
*
*  PROJECT:     Open Faction
*  LICENSE:     See LICENSE in the top level directory
*  FILE:        client/CWeaponSelection.h
*  PURPOSE:     Weapon selection GUI
*  DEVELOPERS:  Rafal Harabien
*
*****************************************************************************/

#ifndef CWEAPONSELECTION_H
#define CWEAPONSELECTION_H

#include <irrlicht.h>
#include "tables/CWeaponsTable.h"
#include "util/CTimer.h"

class CGame;
class CEntity;
class CMultiTexture;
class CWeapon;

class CWeaponSelection: public irr::IEventReceiver
{
    public:
        CWeaponSelection(CGame *pGame);
        ~CWeaponSelection();
        void Render();
        bool OnEvent(const irr::SEvent &Event);
    
    private:
        CGame *m_pGame;
        CMultiTexture *m_Textures[256];
        
        EWeaponType m_WeaponType;
        unsigned m_iWeaponCls;
        
        bool m_bVisible;
        CTimer m_Timer;
        
        static const unsigned VISIBLE_TIME;
        
        CWeapon *FindWeaponFromType(CEntity *pEntity, EWeaponType Type, unsigned iHint = 0u);
};

#endif // CWEAPONSELECTION_H
