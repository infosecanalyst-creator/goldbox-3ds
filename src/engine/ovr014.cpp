#include "engine/engine.h"
#include "platform/platform.h"

namespace engine {

void ovr014::CalculateInitiative(Player player) // sub_3E000
{
    Action action = player->actions;

    action->spell_id = 0;
    action->can_cast = true;
    action->can_use = true;
    action->field_8 = false;
    action->attackIdx = 2;

    reclac_attacks(player);
    gbl::halfActionsLeft = player->baseHalfMoves;

    gbl::resetMovesLeft = false;

    ovr024::CheckAffectsEffect(player, CheckType::Movement);

    player->attack2_AttacksLeft = (byte)ThisRoundActionCount(gbl::halfActionsLeft);

    action->maxSweapTargets = player->attackLevel;

    if (player->in_combat == true)
    {
        action->delay = (sbyte)(ovr024::roll_dice(6, 1) + ovr025::DexReactionAdj(player));

        if (action->delay < 1)
        {
            action->delay = 1;
        }

        if ((((int)player->combat_team + 1) & gbl::area2_ptr->field_596) != 0)
        {
            action->delay -= 6;
        }

        if (action->delay < 0 ||
            action->delay > 20)
        {
            action->delay = 0;
        }
    }
    else
    {
        action->delay = 0;
    }

    player->actions->move = CalcMoves(player);
}


int ovr014::CalcMoves(Player player) // sub_3E124
{
    int moves = player->movement;

    if (player->in_combat == false)
    {
        moves += gbl::area2_ptr->field_6E4;
    }

    if (moves < 1 || moves > 96)
    {
        moves = 1;
    }

    gbl::halfActionsLeft = moves * 2;

    gbl::resetMovesLeft = true;

    ovr024::CheckAffectsEffect(player, CheckType::Movement);

    gbl::resetMovesLeft = false;

    return gbl::halfActionsLeft;
}


void ovr014::sub_3E192(int index, Player target, Player attacker)
{
    gbl::damage = ovr024::roll_dice_save(attacker->attackDiceSize(index), attacker->attackDiceCount(index));
    gbl::damage += attacker->attackDamageBonus(index);

    if (gbl::damage < 0)
    {
        gbl::damage = 0;
    }

    if (CanBackStabTarget(target, attacker) == true)
    {
        gbl::damage *= ((attacker->SkillLevel(SkillType::Thief) - 1) / 4) + 2;
    }

    gbl::damage_flags = (DamageType)0;
    ovr024::CheckAffectsEffect(attacker, CheckType::SpecialAttacks);
    ovr024::CheckAffectsEffect(target, CheckType::Type_5);
}


void ovr014::DisplayAttackMessage(bool attackHits, int attackDamge, int actualDamage, AttackType attack, Player target, Player attacker) /* backstab */
{
    String text;

    if (attack == AttackType::Backstab)
    {
        text = "-Backstabs-";
    }
    else if (attack == AttackType::Slay)
    {
        text = "slays helpless";
    }
    else
    {
        text = "Attacks";
    }

    ovr025::DisplayPlayerStatusString(false, 10, text, attacker);
    int line = 12;

    ovr025::displayPlayerName(false, line, 0x17, target);
    line++;

    if (attack == AttackType::Behind)
    {
        text = "(from behind) ";
    }
    else
    {
        text = String::Empty;
    }

    if (attackHits == true)
    {
        if (attack == AttackType::Slay)
        {
            text = "with one cruel blow";
        }
        else
        {
            text += String("Hitting for ") + ToString(attackDamge);

            if (attackDamge == 1)
            {
                text += " point ";
            }
            else
            {
                text += " points ";
            }

            text += "of damage";

        }

        ovr025::damage_player(actualDamage, target);
    }
    else
    {
        text += "and Misses";
    }

    if (target->health_status != Status::gone)
    {
        seg041::press_any_key(text, true, 10, line + 3, 0x26, line, 0x17);
    }

    line = gbl::textYCol + 1;

    seg041::GameDelay();

    if (actualDamage > 0)
    {
        ovr024::TryLooseSpell(target);
    }

    if (target->in_combat == false)
    {
        ovr025::DisplayPlayerStatusString(false, line, "goes down", target);
        line += 2;

        if (target->health_status == Status::dying)
        {
            seg041::displayString("and is Dying", 0, 10, line, 0x17);
        }

        if (target->health_status == Status::dead ||
            target->health_status == Status::stoned ||
            target->health_status == Status::gone )
        {
            ovr025::DisplayPlayerStatusString(false, line, "is killed", target);
        }

        line += 2;

        ovr024::RemoveCombatAffects(target);

        ovr024::CheckAffectsEffect(target, CheckType::Death);

        if (target->in_combat == false)
        {
            ovr033::CombatantKilled(target);
        }
        else
        {
            seg041::GameDelay();
        }
    }

    ovr025::ClearPlayerTextArea();
}


void ovr014::move_step_into_attack(Player target) /* sub_3E65D */
{
    auto nearTargets = ovr025::BuildNearTargets(1, target);

    if (target->in_combat)
    {
        for (auto cpi : nearTargets)
        {
            Player attacker = cpi->player;

            if (attacker->actions->guarding == true &&
                attacker->IsHeld() == false)
            {
                ovr033::redrawCombatArea(8, 2, ovr033::PlayerMapPos(target));

                attacker->actions->guarding = false;

                RecalcAttacksReceived(target, attacker);

                AttackTarget(nullptr, 0, target, attacker);
            }
        }
    }
}


void ovr014::sub_3E748(int direction, Player player)
{
    int player_index = ovr033::GetPlayerIndex(player);

    Point oldPos = gbl::CombatMap[player_index]->pos;
    Point newPos = oldPos + gbl::MapDirectionDelta[direction];

    // TODO does this solve more problems than it causes? Regarding AI flee
    if (newPos.MapInBounds() == false)
    {
        return;
    }

    int costToMove = 0;
    if ((direction & 0x01) != 0)
    {
        // Diagonal walking...
        costToMove = gbl::BackGroundTiles[gbl::mapToBackGroundTile[newPos]]->move_cost * 3;
    }
    else
    {
        costToMove = gbl::BackGroundTiles[gbl::mapToBackGroundTile[newPos]]->move_cost * 2;
    }

    if (costToMove > player->actions->move)
    {
        player->actions->move = 0;
    }
    else
    {
        player->actions->move -= costToMove;
    }

    byte radius = 1;

    if (player->quick_fight == QuickFight::True)
    {
        radius = 3;

        if (ovr033::CoordOnScreen(newPos - gbl::mapToBackGroundTile->mapScreenTopLeft) == false &&
            gbl::focusCombatAreaOnPlayer == true)
        {
            ovr033::redrawCombatArea(8, 2, oldPos);
        }
    }

    if (gbl::focusCombatAreaOnPlayer == true)
    {
        ovr033::RedrawPlayerBackground(player_index);
    }

    gbl::CombatMap[player_index]->pos = newPos;

    ovr033::setup_mapToPlayerIndex_and_playerScreen();

    if (gbl::focusCombatAreaOnPlayer == true)
    {
        ovr033::redrawCombatArea(8, radius, newPos);
    }

    player->actions->AttacksReceived = 0;
    player->actions->directionChanges = 0;
    seg044::PlaySound(Sound::sound_a);

    move_step_into_attack(player);

    if (player->in_combat == false ||
        player->IsHeld() == true)
    {
        player->actions->move = 0;
    }
}


void ovr014::move_step_away_attack(int direction, Player player) /* sub_3E954 */
{
    auto originAttackers = ovr025::BuildNearTargets(1, player);

    if (originAttackers.Count() == 0)
    {
        return;
    }

    auto combatmap = gbl::CombatMap[ovr033::GetPlayerIndex(player)];

    // move to destination position
    combatmap->pos = combatmap->pos + gbl::MapDirectionDelta[direction]; // C#: combatmap.pos += ...

    auto destAttackers = ovr025::BuildNearTargets(1, player);

    // move back to original position
    combatmap->pos = combatmap->pos - gbl::MapDirectionDelta[direction]; // C#: combatmap.pos -= ...

    // remove attackers from both locations
    for (auto cpiB : destAttackers)
    {
        originAttackers.RemoveAll([&](const CombatPlayerIndex& cpiA) { return cpiA->player == cpiB->player; });
    }

    if (player->in_combat == false)
    {
        //what the heck are we doing here then?
        // and why is this test not earlier in the function.
        //throw new System.NotSupportedException();
        return;
    }

    for (auto cpiA : originAttackers)
    {
        gbl::display_hitpoints_ac = true;
        gbl::focusCombatAreaOnPlayer = true;
        bool found = false;

        Player attacker = cpiA->player;

        if (attacker->IsHeld() == false &&
            CanSeeTargetA(player, attacker) == true &&
            attacker->HasAffect(Affects::weap_dragon_slayer) == false &&
            attacker->HasAffect(Affects::affect_4a) == false)
        {
            int end_dir = attacker->actions->direction + 10;

            for (int tmpDir = attacker->actions->direction + 6; tmpDir <= end_dir; tmpDir++)
            {
                if (found == false)
                {
                    if (attacker->actions->delay > 0 ||
                        attacker->actions->AttacksReceived == 0 ||
                        ovr032::CanSeeCombatant(tmpDir % 8, ovr033::PlayerMapPos(player), ovr033::PlayerMapPos(attacker)) == true)
                    {
                        byte attackIndex = 1;
                        if (attacker->attacksCount == 0)
                        {
                            attackIndex = 2;
                        }

                        if (attacker->attack1_AttacksLeft > 0)
                        {
                            attackIndex = 1;
                        }

                        if (attacker->attack2_AttacksLeft > 0)
                        {
                            attackIndex = 2;
                        }

                        if (attacker->AttacksLeft(attackIndex) == 0)
                        {
                            attacker->AttacksLeftSet(attackIndex, 1);
                        }

                        attacker->actions->attackIdx = attackIndex;

                        Player backupTarget = attacker->actions->target;

                        AttackTarget(nullptr, 1, player, attacker);
                        found = true;

                        attacker->actions->target = backupTarget;

                        if (player->in_combat == true)
                        {
                            gbl::display_hitpoints_ac = true;
                            ovr025::CombatDisplayPlayerSummary(player);
                        }
                    }
                }
            }
        }
    }
}



void ovr014::flee_battle(Player player)
{
    bool gets_away = false;

    if (ovr025::BuildNearTargets(0xff, player).Count() == 0)
    {
        gets_away = true;
    }
    else
    {
        int var_4 = CalcMoves(player) / 2;
        int var_3 = MaxOppositionMoves(player);

        if (var_3 < var_4)
        {
            gets_away = true;
        }
        else if (var_3 == var_4 && ovr024::roll_dice(2, 1) == 1)
        {
            gets_away = true;
        }
    }

    if (gets_away == true)
    {
        ovr024::RemoveFromCombat("Got Away", Status::running, player);
    }
    else
    {
        ovr025::string_print01("Escape is blocked");
    }

    ovr025::clear_actions(player);
}


void ovr014::reclac_attacks(Player player) // sub_3EDD4
{
    bool foundRanged = false;
    Item rangedItem = nullptr;
    int origAttacks = player->attack1_AttacksLeft;
    player->attack1_AttacksLeft = player->attacksCount;

    if (ovr025::is_weapon_ranged(player) == true &&
        ovr025::GetCurrentAttackItem(rangedItem, player) == true)
    {
        foundRanged = true;
        int numAttacks = gbl::ItemDataTable[player->activeItems->primaryWeapon()->type]->numberAttacks;

        if (numAttacks < 2)
        {
            numAttacks = 2;
        }

        gbl::halfActionsLeft = numAttacks;
    }
    else
    {
        gbl::halfActionsLeft = player->attack1_AttacksLeft;
    }

    gbl::resetMovesLeft = false;
    ovr024::CheckAffectsEffect(player, CheckType::Movement);

    int attacks = ThisRoundActionCount(gbl::halfActionsLeft);

    if (foundRanged == true &&
        rangedItem != nullptr)
    {
        int rangedAmmo = 1;
        if (rangedItem->count > rangedAmmo)
        {
            rangedAmmo = rangedItem->count;
        }

        if (rangedAmmo < attacks &&
            rangedItem->count > 0)
        {
            attacks = rangedAmmo;
        }
    }

    if (player->actions->field_8 == false ||
        attacks < origAttacks ||
        (player->actions->field_8 == true &&
          attacks < (origAttacks * 2) &&
          foundRanged == false))
    {
        player->attack1_AttacksLeft = (byte)attacks;
    }
}


int ovr014::ThisRoundActionCount(int halfActionsLeft) // sub_3EF0D
{
    if ((gbl::combat_round & 1) == 1)
    {
        halfActionsLeft++;
    }

    return halfActionsLeft / 2;
}


bool ovr014::TrySweepAttack(Player target, Player attacker) // sub_3EF3D
{
    if (attacker->attack1_AttacksLeft < attacker->actions->maxSweapTargets &&
        target->HitDice == 0 &&
        ovr025::getTargetRange(target, attacker) == 1)
    {
        auto nearTargets = ovr025::BuildNearTargets(1, attacker);

        auto targetepi = nearTargets.Find([&](const CombatPlayerIndex& epi) { return epi->player == target; });
        int sweepableCount = nearTargets.FindAll([&](const CombatPlayerIndex& epi) { return epi->player->HitDice == 0; }).Count();

        if (sweepableCount > attacker->attack1_AttacksLeft)
        {
            if (sweepableCount > attacker->actions->maxSweapTargets)
            {
                sweepableCount = attacker->actions->maxSweapTargets;
            }

            ovr025::DisplayPlayerStatusString(true, 10, "sweeps", attacker);

            nearTargets.Remove(targetepi);
            nearTargets.Insert(0, targetepi);

            for (auto sweepepi : nearTargets.FindAll([&](const CombatPlayerIndex& e) { return e->player->hitBonus == 0; }).GetRange(0, sweepableCount))
            {
                auto sweeptarget = sweepepi->player;
                RecalcAttacksReceived(sweeptarget, attacker);

                attacker->attack1_AttacksLeft = 1;

                AttackTarget(nullptr, 0, sweeptarget, attacker);
            }

            return true;
        }
    }

    return false;
}


bool ovr014::CanSeeTargetA(Player targetA, Player targetB) //sub_3F143 
{
    if (targetA != nullptr)
    {
        if (targetB == targetA)
        {
            return true;
        }
        else
        {
            gbl::targetInvisible = false;

            ovr024::CheckAffectsEffect(targetA, CheckType::Visibility);

            if (gbl::targetInvisible == false)
            {
                auto old_target = targetB->actions->target;

                targetB->actions->target = targetA;

                ovr024::CheckAffectsEffect(targetB, CheckType::None);

                targetB->actions->target = old_target;
            }

            return (gbl::targetInvisible == false);
        }
    }

    return false;
}

// static sbyte[] /*seg600:0369*/ unk_16679 -- defined in decl/ovr014.h

void ovr014::turns_undead(Player player)
{
    ovr025::DisplayPlayerStatusString(false, 10, "turns undead...", player);
    ovr027::ClearPromptArea();
    seg041::GameDelay();

    bool any_turned = false;
    byte var_6 = 0;

    player->actions->hasTurnedUndead = true;

    byte var_3 = 6;
    int var_2 = ovr024::roll_dice(12, 1);
    int var_1 = ovr024::roll_dice(20, 1);

    int clericLvl = player->SkillLevel(SkillType::Cleric);

    byte var_B;

    if (clericLvl >= 1 && clericLvl <= 8)
    {
        var_B = player->cleric_lvl();
    }
    else if (clericLvl >= 9 && clericLvl <= 13)
    {
        var_B = 9;
    }
    else
    {
        var_B = 10;
    }

    Player target;
    while (FindLowestE9Target(target, player) == true && var_2 > 0 && var_6 == 0)
    {
        int var_4 = unk_16679[(target->field_E9 * 10) + var_B];

        if (var_1 >= Math::Abs(var_4))
        {
            any_turned = true;

            ovr033::RedrawCombatIfFocusOn(false, 3, target);
            gbl::display_hitpoints_ac = true;
            ovr025::CombatDisplayPlayerSummary(target);

            if (var_4 > 0)
            {
                target->actions->fleeing = true;
                ovr025::MagicAttackDisplay("is turned", true, target);
            }
            else
            {
                ovr025::DisplayPlayerStatusString(false, 10, "Is destroyed", target);
                ovr033::CombatantKilled(target);
                target->health_status = Status::gone;
                target->in_combat = false;
            }

            if (var_3 > 0)
            {
                var_3 -= 1;
            }

            var_2 -= 1;

            if (var_2 == 0 && var_3 > 0 && var_4 < 0)
            {
                var_2++;
            }

            ovr025::ClearPlayerTextArea();
        }
        else
        {
            var_6 = 1;
        }
    }

    if (any_turned == false)
    {
        ovr025::string_print01("Nothing Happens...");
    }

    ovr025::CountCombatTeamMembers();
    ovr025::clear_actions(player);

    ovr025::ClearPlayerTextArea();
}


bool ovr014::FindLowestE9Target(Player& output, Player player) /* sub_3F433 */
{
    output = nullptr;

    byte minE9 = 13;

    auto nearTargets = ovr025::BuildNearTargets(0xff, player);
    bool result = false;

    for (auto epi : nearTargets)
    {
        Player target = epi->player;

        if (target->actions->fleeing == false &&
            target->field_E9 > 0 &&
            target->field_E9 < minE9)
        {
            minE9 = target->field_E9;
            output = target;
            result = true;
        }
    }

    return result;
}


bool ovr014::AttackTarget01(Item item, int arg_8, Player target, Player attacker) // sub_3F4EB
{
    int target_ac;
    bool turnComplete = true;
    bool BehindAttack = arg_8 != 0;
    turnComplete = false;
    gbl::bytes_1D2C9[1] = 0;
    gbl::bytes_1D2C9[2] = 0;
    gbl::bytes_1D900[1] = 0;
    gbl::bytes_1D900[2] = 0;
    bool var_11 = false;
    bool targetNotInCombat = false;
    gbl::damage = 0;

    attacker->actions->field_8 = true;

    if (target->IsHeld() == true)
    {
        seg044::PlaySound(Sound::sound_attackHeld);

        while (attacker->AttacksLeft(attacker->actions->attackIdx) == 0)
        {
            attacker->actions->attackIdx--;
        }

        gbl::bytes_1D900[attacker->actions->attackIdx] += 1;

        DisplayAttackMessage(true, 1, target->hit_point_current + 5, AttackType::Slay, target, attacker);
        ovr024::remove_invisibility(attacker);

        attacker->attack1_AttacksLeft = 0;
        attacker->attack2_AttacksLeft = 0;

        var_11 = true;
        turnComplete = true;
    }
    else
    {
        if (attacker->activeItems->primaryWeapon() != nullptr &&
            (target->field_DE > 0x80 || (target->field_DE & 7) > 1))
        {
            ItemData itemData = gbl::ItemDataTable[attacker->activeItems->primaryWeapon()->type];

            attacker->attack1_DiceCount = itemData->diceCountLarge;
            attacker->attack1_DiceSize = itemData->diceSizeLarge;
            attacker->attack1_DamageBonus -= itemData->bonusNormal;
            attacker->attack1_DamageBonus += itemData->bonusLarge;
        }

        ovr025::reclac_player_values(target);
        ovr024::CheckAffectsEffect(target, CheckType::Type_11);

        if (CanBackStabTarget(target, attacker) == true)
        {
            target_ac = target->ac_behind - 4;
        }
        else
        {
            if (target->actions->AttacksReceived > 1 &&
                getTargetDirection(target, attacker) == target->actions->direction &&
                target->actions->directionChanges > 4)
            {
                BehindAttack = true;
            }

            if (BehindAttack == true)
            {
                target_ac = target->ac_behind;
            }
            else
            {
                target_ac = target->ac;
            }
        }

        target_ac += RangedDefenseBonus(target, attacker);
        AttackType attack_type = AttackType::Normal;
        if (BehindAttack == true)
        {
            attack_type = AttackType::Behind;
        }

        if (CanBackStabTarget(target, attacker) == true)
        {
            attack_type = AttackType::Backstab;
        }

        for (int attackIdx = attacker->actions->attackIdx; attackIdx >= 1; attackIdx--)
        {
            while (attacker->AttacksLeft(attackIdx) > 0 &&
                targetNotInCombat == false)
            {
                attacker->AttacksLeftDec(attackIdx);
                attacker->actions->attackIdx = attackIdx;

                gbl::bytes_1D900[attackIdx] += 1;

                if (ovr024::PC_CanHitTarget(target_ac, target, attacker) ||
                    target->IsHeld() == true)
                {
                    gbl::bytes_1D2C9[attackIdx] += 1;

                    seg044::PlaySound(Sound::sound_attackHeld);
                    var_11 = true;
                    sub_3E192(attackIdx, target, attacker);
                    DisplayAttackMessage(true, gbl::damage, gbl::damage, attack_type, target, attacker);

                    if (target->in_combat == true)
                    {
                        ovr024::CheckAffectsEffect(attacker, (CheckType)attackIdx + 1);
                    }

                    if (target->in_combat == false)
                    {
                        targetNotInCombat = true;
                    }

                    if (attacker->in_combat == false)
                    {
                        attacker->AttacksLeftSet(attackIdx, 0);
                    }
                }
            }
        }

        if (item != nullptr &&
            item->count == 0 &&
            item->type == ItemType::DartOfHornetsNest)
        {
            attacker->attack1_AttacksLeft = 0;
            attacker->attack2_AttacksLeft = 0;
        }

        if (var_11 == false)
        {
            seg044::PlaySound(Sound::sound_9);
            DisplayAttackMessage(false, 0, 0, attack_type, target, attacker);
        }

        turnComplete = true;
        if (attacker->attack1_AttacksLeft > 0 ||
            attacker->attack2_AttacksLeft > 0)
        {
            turnComplete = false;
        }

        attacker->actions->maxSweapTargets = 0;
    }

    if (attacker->in_combat == false)
    {
        turnComplete = true;
    }

    if (turnComplete == true)
    {
        ovr025::clear_actions(attacker);
    }

    return turnComplete;
}


void ovr014::RecalcAttacksReceived(Player target, Player attacker) // sub_3F94D
{
    target->actions->AttacksReceived++;

    int targetDir = getTargetDirection(attacker, target);

    int dirDiff = ((targetDir - target->actions->direction) + 8) % 8;

    if (dirDiff > 4)
    {
        dirDiff = 8 - dirDiff;
    }

    target->actions->directionChanges = (target->actions->directionChanges + dirDiff) % 8;
}


bool ovr014::AttackTarget(Item rangedWeapon, int attackType, Player target, Player attacker) // sub_3F9DB
{
    int dir = 0;

    gbl::focusCombatAreaOnPlayer = true;
    gbl::display_hitpoints_ac = true;

    gbl::combat_round_no_action_limit = gbl::combat_round + gbl::combat_round_no_action_value;

    if (target->actions->AttacksReceived < 2 && attackType == 0)
    {
        dir = getTargetDirection(attacker, target);

        target->actions->direction = (dir + 4) % 8;
    }
    else if (ovr033::PlayerOnScreen(false, target) == true)
    {
        dir = target->actions->direction;

        if (attackType == 0)
        {
            target->actions->direction = (dir + 4) % 8;
        }
    }

    if (ovr033::PlayerOnScreen(false, target) == true)
    {
        ovr033::draw_74B3F(false, Icon::Normal, dir, target);
    }

    dir = getTargetDirection(target, attacker);
    ovr025::CombatDisplayPlayerSummary(attacker);

    ovr033::draw_74B3F(false, Icon::Attack, dir, attacker);

    attacker->actions->target = target;

    seg049::SysDelay(100);

    if (rangedWeapon != nullptr)
    {
        DrawRangedAttack(rangedWeapon, target, attacker);
    }

    if (attacker->activeItems->primaryWeapon() != nullptr && 
        (attacker->activeItems->primaryWeapon()->type == ItemType::Sling ||
        attacker->activeItems->primaryWeapon()->type == ItemType::StaffSling))
    {
        DrawRangedAttack(attacker->activeItems->primaryWeapon(), target, attacker);
    }

    bool turnComplete = true;

    if (attacker->attack1_AttacksLeft > 0 ||
        attacker->attack2_AttacksLeft > 0)
    {
        Player player_bkup = gbl::SelectedPlayer;

        gbl::SelectedPlayer = attacker;

        turnComplete = AttackTarget01(rangedWeapon, attackType, target, attacker);

        if (rangedWeapon != nullptr)
        {
            if (rangedWeapon->count > 0)
            {
                rangedWeapon->count = gbl::bytes_1D900[1];
            }

            if (rangedWeapon->count == 0)
            {
                if (ovr025::is_weapon_ranged_melee(attacker) == true &&
                    rangedWeapon->affect_3 != Affects::affect_89)
                {
                    Item new_item = rangedWeapon->ShallowClone();
                    new_item->readied = false;

                    gbl::items_pointer.Add(new_item);

                    ovr025::lose_item(rangedWeapon, attacker);
                    gbl::item_ptr = new_item;
                }
                else
                {
                    ovr025::lose_item(rangedWeapon, attacker);
                }
            }
        }

        ovr025::reclac_player_values(attacker);
        gbl::SelectedPlayer = player_bkup;
    }

    if (turnComplete == true)
    {
        ovr025::clear_actions(attacker);
    }

    if (ovr033::PlayerOnScreen(false, attacker) == true)
    {
        ovr033::draw_74B3F(true, Icon::Attack, attacker->actions->direction, attacker);
        ovr033::draw_74B3F(false, Icon::Normal, attacker->actions->direction, attacker);
    }

    return turnComplete;
}


int ovr014::RangedDefenseBonus(Player target, Player attacker) /* sub_3FCED */
{
    if (ovr025::is_weapon_ranged(attacker) == true)
    {
        int range = ovr025::getTargetRange(target, attacker);

        int oneThirdRange = (gbl::ItemDataTable[attacker->activeItems->primaryWeapon()->type]->range - 1) / 3;
        int acAdjustment = 0;

        if (range > oneThirdRange)
        {
            range -= oneThirdRange;
            acAdjustment += 2;
        }

        if (range > oneThirdRange)
        {
            range -= oneThirdRange;
            acAdjustment += 3;
        }

        return acAdjustment;
    }
    else
    {
        return 0;
    }
}

bool ovr014::find_healing_target(Player& target, Player healer) /* sub_3FDFE */
{
    Player lowest_target = nullptr;
    int lowest_hp = 0x0FF;
    DownedPlayerTile var_8 = nullptr;

    for (int dir = 0; dir <= 8; dir++)
    {
        auto map = gbl::MapDirectionDelta[dir] + ovr033::PlayerMapPos(healer);

        int ground_tile;
        int player_index;
        ovr033::AtMapXY(ground_tile, player_index, map);

        if (player_index > 0)
        {
            target = gbl::player_array[player_index];

            if (target->combat_team == healer->combat_team &&
                target->hit_point_current < target->hit_point_max)
            {
                if (target->hit_point_current < lowest_hp ||
                    (target == healer && target->hit_point_current < (target->hit_point_max / 2)))
                {
                    lowest_target = target;
                    lowest_hp = target->hit_point_current;
                }
            }
        }
        else if (ground_tile == gbl::Tile_DownPlayer)
        {
            var_8 = gbl::downedPlayers.FindLast([&](const DownedPlayerTile& cell) { return cell->target != nullptr && cell->map == map &&
                cell->target->health_status != Status::tempgone && cell->target->health_status != Status::running &&
                cell->target->health_status != Status::unconscious; });
        }
    }

    if (lowest_hp < 8 ||
        var_8 == nullptr)
    {
        target = lowest_target;
    }
    else
    {
        target = var_8->target;
    }

    bool target_found = (target != nullptr);

    return target_found;
}

// static Affects[] unk_18ADB -- defined in decl/ovr014.h

bool ovr014::sub_4001C(DownedPlayerTile arg_0, bool canTargetEmptyGround, QuickFight quick_fight, int spellId)
{
    bool var_2 = false;
    if (quick_fight == QuickFight::False)
    {
        bool allowTarget = spellId != 0x53;

        var_2 = aim_menu(arg_0, allowTarget, canTargetEmptyGround, false, ovr023::SpellRange(spellId), gbl::SelectedPlayer);
        gbl::SelectedPlayer->actions->target = arg_0->target;
    }
    else if (gbl::spellCastingTable[spellId]->field_E == 0)
    {
        arg_0->target = gbl::SelectedPlayer;

        if (spellId != 3 || find_healing_target(arg_0->target, gbl::SelectedPlayer))
        {
            arg_0->map = ovr033::PlayerMapPos(arg_0->target);
            var_2 = true;
        }
    }
    else
    {
        int var_9 = 1;

        while (var_9 > 0 &&
                var_2 == false)
        {
            bool var_3 = true;

            if (find_target(true, 0, ovr023::SpellRange(spellId), gbl::SelectedPlayer) == true)
            {
                Player target = gbl::SelectedPlayer->actions->target;

                if (target->IsHeld() == true)
                {
                    for (int i = 1; i <= 4; i++)
                    {
                        if (gbl::spellCastingTable[spellId]->affect_id == unk_18ADB[i])
                        {
                            var_3 = false;
                        }
                    }
                }

                if (var_3 == true)
                {
                    arg_0->target = gbl::SelectedPlayer->actions->target;
                    arg_0->map = ovr033::PlayerMapPos(arg_0->target);
                    var_2 = true;
                }
            }

            var_9 -= 1;
        }
    }


    if (var_2 == true)
    {
        gbl::targetPos = arg_0->map;
    }
    else
    {
        arg_0->Clear();
    }

    return var_2;
}

bool ovr014::target(QuickFight quick_fight, int spellId)
{
    DownedPlayerTile var_C = New<DownedPlayerTile_>();

    bool castSpell = true;
    gbl::spellTargets.Clear();
    gbl::byte_1D2C7 = false;

    gbl::targetPos = ovr033::PlayerMapPos(gbl::SelectedPlayer);

    int tmp1 = gbl::spellCastingTable[spellId]->field_6 & 0x0F;

    if (tmp1 == 0)
    {
        gbl::spellTargets.Clear();
        gbl::spellTargets.Add(gbl::SelectedPlayer);
    }
    else if (tmp1 == 5)
    {
        int var_5 = 0;
        gbl::spellTargets.Clear();

        int var_4;

        if (spellId == 0x4F)
        {
            var_4 = ovr025::spellMaxTargetCount(0x4F);
        }
        else
        {
            var_4 = ovr024::roll_dice(4, 2);
        }

        bool stop_loop = false;

        do
        {
            if (sub_4001C(var_C, false, quick_fight, spellId) == true)
            {
                bool found = gbl::spellTargets.Exists([&](const Player& st) { return st == var_C->target; });

                if (found == false)
                {
                    Player target = var_C->target;
                    gbl::spellTargets.Add(target);

                    gbl::targetPos = ovr033::PlayerMapPos(var_C->target);

                    if (spellId != 0x4f)
                    {
                        byte hitDice = target->HitDice;

                        if (hitDice == 0 || hitDice == 1)
                        {
                            var_5 += 1;
                        }
                        else if (hitDice == 2)
                        {
                            var_5 += 2;
                        }
                        else if (hitDice == 3)
                        {
                            var_5 += 4;
                        }
                        else
                        {
                            var_5 += 8;
                        }
                    }
                    else
                    {
                        byte al = target->field_DE;

                        if (al == 1)
                        {
                            var_5 += 1;
                        }
                        else if (al == 2 || al == 3)
                        {
                            var_5 += 2;
                        }
                        else if (al == 4)
                        {
                            var_5 += 4;
                        }
                    }

                    if (gbl::spellTargets.Count() > 0 && var_5 > var_4)
                    {
                        stop_loop = true;
                    }
                }
                else
                {
                    if (quick_fight != (QuickFight)0)
                    {
                        var_4 -= 1;
                    }
                    else
                    {
                        ovr025::string_print01("Already been targeted");
                    }
                }

                ovr033::RedrawPosition(ovr033::PlayerMapPos(var_C->target));
            }
            else
            {
                stop_loop = true;
            }
        } while (stop_loop == false && var_4 != 0);
    }
    else if (tmp1 == 0x0F)
    {
        if (sub_4001C(var_C, false, quick_fight, spellId) == true)
        {
            if (gbl::SelectedPlayer->actions->target != nullptr)
            {
                gbl::spellTargets.Clear();
                gbl::spellTargets.Add(gbl::SelectedPlayer->actions->target);
            }
            else
            {
                /* TODO it doesn't make sense to mask the low nibble then shift it out */
                auto scl = ovr032::Rebuild_SortedCombatantList(1, (gbl::spellCastingTable[spellId]->field_6 & 0x0f) >> 4, gbl::targetPos, [](Player sc) { return true; });

                gbl::spellTargets.Clear();
                for (auto sc : scl)
                {
                    gbl::spellTargets.Add(sc->player);
                }
                gbl::byte_1D2C7 = true;
            }
        }
        else
        {
            castSpell = false;
        }
    }
    else if (tmp1 >= 8 && tmp1 <= 0x0E)
    {
        if (sub_4001C(var_C, true, quick_fight, spellId) == true)
        {
            auto scl = ovr032::Rebuild_SortedCombatantList(1, gbl::spellCastingTable[spellId]->field_6 & 7, gbl::targetPos, [](Player sc) { return true; });

            gbl::spellTargets.Clear();
            for (auto sc : scl)
            {
                gbl::spellTargets.Add(sc->player);
            }

            gbl::byte_1D2C7 = true;
        }
        else
        {
            castSpell = false;
        }
    }
    else
    {
        int max_targets = (gbl::spellCastingTable[spellId]->field_6 & 3) + 1;
        gbl::spellTargets.Clear();

        while (max_targets > 0)
        {
            if (sub_4001C(var_C, false, quick_fight, spellId) == true)
            {
                bool found = gbl::spellTargets.Exists([&](const Player& st) { return st == var_C->target; });

                if (found == false)
                {
                    gbl::spellTargets.Add(var_C->target);
                    max_targets -= 1;

                    gbl::targetPos = ovr033::PlayerMapPos(var_C->target);
                }
                else
                {
                    if (quick_fight == (QuickFight)0)
                    {
                        ovr025::string_print01("Already been targeted");
                    }
                    else
                    {
                        max_targets -= 1;
                    }
                }

                ovr033::RedrawPosition(ovr033::PlayerMapPos(var_C->target));
            }
            else
            {
                max_targets = 0;
            }
        }

        if (gbl::spellTargets.Count() == 0)
        {
            castSpell = false;
            gbl::targetPos = Point();
        }

        //gbl.targetPos = ovr033.PlayerMapPos(gbl.spellTargets[gbl.spellTargets.Count-1]);
    }

    return castSpell;
}


void ovr014::spell_menu3(bool& casting_spell, QuickFight quick_fight, int spell_id)
{
    Player player = gbl::SelectedPlayer;
    bool var_6 = true;
    int var_5 = -1;
    casting_spell = false;

    if (spell_id == 0)
    {
        spell_id = ovr020::spell_menu2(var_6, var_5, SpellSource::Cast, SpellLoc::memory);
    }

    if (spell_id > 0 &&
        gbl::spellCastingTable[spell_id]->whenCast == SpellWhen::Camp)
    {
        ovr025::string_print01("Camp Only Spell");
        spell_id = 0;
    }

    if (quick_fight == QuickFight::False)
    {
        ovr025::RedrawCombatScreen();
        gbl::focusCombatAreaOnPlayer = true;
        gbl::display_hitpoints_ac = true;

        ovr033::RedrawCombatIfFocusOn(true, 3, player);
        ovr025::CombatDisplayPlayerSummary(player);
    }

    if (spell_id > 0)
    {
        sbyte delay = (sbyte)(gbl::spellCastingTable[spell_id]->castingDelay / 3);

        if (delay == 0)
        {
            ovr023::sub_5D2E1(true, quick_fight, spell_id);

            casting_spell = true;
            ovr025::clear_actions(player);
        }
        else
        {
            casting_spell = true;
            ovr025::DisplayPlayerStatusString(true, 10, "Begins Casting", player);

            player->actions->spell_id = spell_id;

            if (player->actions->delay > delay)
            {
                player->actions->delay = delay;
            }
            else
            {
                player->actions->delay = 1;
            }
        }
    }
}


bool ovr014::CanBackStabTarget(Player target, Player attacker) /* sub_408D7 */
{
    if (attacker->SkillLevel(SkillType::Thief) > 0)
    {
        Item weapon = attacker->activeItems->primaryWeapon();

        if (weapon == nullptr ||
            weapon->type == ItemType::DrowLongSword ||
            weapon->type == ItemType::Club ||
            weapon->type == ItemType::Dagger ||
            weapon->type == ItemType::BroadSword ||
            weapon->type == ItemType::LongSword ||
            weapon->type == ItemType::ShortSword)
        {
            if (target->actions->AttacksReceived > 1 &&
                (target->field_DE & 0x7F) <= 1 &&
                getTargetDirection(target, attacker) == target->actions->direction)
            {
                return true;
            }
        }
    }

    return false;
}


byte ovr014::getTargetDirection(Player playerB, Player playerA) /* sub_409BC */
{
    Point plyr_a = ovr033::PlayerMapPos(playerA);
    Point plyr_b = ovr033::PlayerMapPos(playerB);

    int diff_x = Math::Abs(plyr_b.x - plyr_a.x);
    int diff_y = Math::Abs(plyr_b.y - plyr_a.y);

    byte direction = 0;
    bool solved = false;

    while (solved == false)
    {
        switch (direction)
        {
            case 0:
                if (plyr_b.y > plyr_a.y ||
                    ((0x26A * diff_x) / 0x100) > diff_y)
                {
                    solved = false;
                }
                else
                {
                    solved = true;
                }
                break;

            case 2:
                if (plyr_b.x < plyr_a.x ||
                    ((0x6A * diff_x) / 0x100) < diff_y)
                {
                    solved = false;
                }
                else
                {
                    solved = true;
                }
                break;

            case 4:
                if (plyr_b.y < plyr_a.y ||
                    ((0x26A * diff_x) / 0x100) > diff_y)
                {
                    solved = false;
                }
                else
                {
                    solved = true;
                }
                break;

            case 6:
                if (plyr_b.x > plyr_a.x ||
                    ((0x6A * diff_x) / 0x100) < diff_y)
                {
                    solved = false;
                }
                else
                {
                    solved = true;
                }
                break;

            case 1:
                if (plyr_b.y > plyr_a.y ||
                    plyr_b.x < plyr_a.x ||
                    ((0x26A * diff_x) / 0x100) < diff_y ||
                    ((0x6A * diff_x) / 0x100) > diff_y)
                {
                    solved = false;
                }
                else
                {
                    solved = true;
                }
                break;

            case 3:
                if (plyr_b.y < plyr_a.y ||
                    plyr_b.x < plyr_a.x ||
                    ((0x26a * diff_x) / 0x100) < diff_y ||
                    ((0x6a * diff_x) / 0x100) > diff_y)
                {
                    solved = false;
                }
                else
                {
                    solved = true;
                }
                break;

            case 5:
                if (plyr_b.y < plyr_a.y ||
                    plyr_b.x > plyr_a.x ||
                    ((0x26a * diff_x) / 0x100) < diff_y ||
                    ((0x6a * diff_x) / 0x100) > diff_y)
                {
                    solved = false;
                }
                else
                {
                    solved = true;
                }
                break;

            case 7:
                if (plyr_b.y > plyr_a.y ||
                    plyr_b.x > plyr_a.x ||
                    ((0x26a * diff_x) / 0x100) < diff_y ||
                    ((0x6a * diff_x) / 0x100) > diff_y)
                {
                    solved = false;
                }
                else
                {
                    solved = true;
                }
                break;
        }

        if (solved == false)
        {
            direction++;
        }
    }

    return direction;
}


void ovr014::DrawRangedAttack(Item item, Player target, Player attacker) /* sub_40BF1 */
{
    seg044::PlaySound(Sound::sound_c);

    int dir = getTargetDirection(target, attacker);

    int frame_count = 1;
    int delay = 10;
    int iconId = 13;

    switch (item->type)
    {
        case ItemType::Dart:
        case ItemType::Javelin:
        case ItemType::DartOfHornetsNest:
        case ItemType::Quarrel:
        case ItemType::Spear:
        case ItemType::Arrow:
            if ((dir & 1) == 1)
            {
                if (dir == 3 || dir == 5)
                {
                    ovr025::load_missile_dax((dir == 5), 0, Icon::Attack, iconId + 1);
                }
                else
                {
                    ovr025::load_missile_dax((dir == 7), 0, Icon::Normal, iconId + 1);
                }
            }
            else
            {
                if (dir >= 4)
                {
                    ovr025::load_missile_dax(false, 0, Icon::Attack, iconId + (dir % 4));
                }
                else
                {
                    ovr025::load_missile_dax(false, 0, Icon::Normal, iconId + (dir % 4));
                }
            }
            seg044::PlaySound(Sound::sound_c);
            break;

        case ItemType::HandAxe:
        case ItemType::Club:
        case ItemType::Glaive:
            ovr025::load_missile_icons(iconId + 3);
            frame_count = 4;
            delay = 50;
            seg044::PlaySound(Sound::sound_9);
            break;

        case ItemType::Type_85:
        case ItemType::FlaskOfOil:
            ovr025::load_missile_icons(iconId + 4);
            frame_count = 4;
            delay = 50;
            seg044::PlaySound(Sound::sound_6);
            break;

        case ItemType::StaffSling:
        case ItemType::Sling:
        case ItemType::Spine:
            iconId++;
            ovr025::load_missile_dax(false, 0, Icon::Normal, iconId + 7);
            ovr025::load_missile_dax(false, 1, Icon::Attack, iconId + 7);
            frame_count = 2;
            delay = 10;
            seg044::PlaySound(Sound::sound_6);
            break;

        default:
            ovr025::load_missile_dax(false, 0, Icon::Normal, iconId + 7);
            ovr025::load_missile_dax(false, 1, Icon::Attack, iconId + 7);
            frame_count = 2;
            delay = 20;
            seg044::PlaySound(Sound::sound_9);
            break;
    }

    ovr025::draw_missile_attack(delay, frame_count, ovr033::PlayerMapPos(target), ovr033::PlayerMapPos(attacker));
}


void ovr014::calc_enemy_health_percentage() /* sub_40E00 */
{
    int maxTotal = 0;
    int currentTotal = 0;

    for (Player player : gbl::TeamList)
    {
        if (player->combat_team == CombatTeam::Enemy)
        {
            if (player->in_combat == true)
            {
                currentTotal += player->hit_point_current;
            }

            maxTotal += player->hit_point_max;
        }
    }

    if (maxTotal > 0)
    {
        gbl::enemyHealthPercentage = ((20 * currentTotal) / maxTotal) * 5;
    }
}


int ovr014::MaxOppositionMoves(Player player) // sub_40E8F
{
    int maxMoves = 0;

    for (Player mob : gbl::TeamList)
    {
        if (player->OppositeTeam() == mob->combat_team && mob->in_combat == true)
        {
            int moves = CalcMoves(mob) / 2;

            maxMoves = Math::Max(moves, maxMoves);
        }
    }

    return maxMoves;
}


bool ovr014::can_attack_target(Player target, Player attacker) /* sub_40F1F */
{
    bool result;

    if (target->OppositeTeam() == attacker->combat_team ||
        attacker->quick_fight == QuickFight::True)
    {
        result = true;
    }
    else if (ovr027::yes_no(gbl::defaultMenuColors, "Attack Ally: ") != 'Y')
    {
        result = false;
    }
    else
    {
        result = true;
        gbl::area2_ptr->field_666 = 1;

        for (Player player : gbl::TeamList)
        {
            if (player->health_status == Status::okey &&
                player->control_morale >= Control::NPC_Base)
            {
                player->combat_team = CombatTeam::Enemy;
                player->actions->target = nullptr;
            }
        }

        ovr025::CountCombatTeamMembers();
    }

    return result;
}


char ovr014::aim_sub_menu(bool showTarget, bool showRange, int maxRange, Player target, Player attacker) /* Aim_menu */
{
    String text = String::Empty;
    int range = ovr025::getTargetRange(target, attacker);
    int direction = getTargetDirection(target, attacker);

    if (showRange == true)
    {
        String range_txt = String("Range = ") + ToString(range) + "  ";
        seg041::displayString(range_txt, 0, 10, 0x17, 0);
    }

    if (range <= maxRange)
    {
        if (showRange == false)
        {
            if (showTarget == true)
            {
                text = "Target ";
            }
            else
            {
                text = String::Empty;
            }
        }
        else if (target != attacker)
        {
            if (ovr025::is_weapon_ranged(attacker) == false)
            {
                text = "Target ";
            }
            else
            {
                Item dummyItem;
                if (ovr025::GetCurrentAttackItem(dummyItem, attacker) == true &&
                    (ovr025::BuildNearTargets(1, attacker).Count() == 0 || ovr025::is_weapon_ranged_melee(attacker) == true))
                {
                    text = "Target ";
                }
            }
        }
    }

    text = String("Next Prev Manual ") + text + "Center Exit";
    ovr033::RedrawCombatIfFocusOn(true, 3, target);
    gbl::display_hitpoints_ac = true;
    ovr025::CombatDisplayPlayerSummary(target);

    {   // goldbox-3ds: tell the host where the aim is (display-only)
        auto tp = ovr033::PlayerMapPos(target);
        auto tl = gbl::mapToBackGroundTile->mapScreenTopLeft;
        platform::AimShow(0, tp.x, tp.y, tl.x, tl.y);
    }
    char input_key = ovr027::displayInput(false, 1, gbl::defaultMenuColors, text, "Aim:");
    platform::AimHide();

    return input_key;
}


bool ovr014::sub_411D8(DownedPlayerTile arg_0, bool showRange, Player target, Player attacker)
{
    bool arg_4 = true;

    if (showRange &&
        can_attack_target(target, attacker) == false)
    {
        arg_4 = false;
    }

    if (arg_4 == true)
    {
        arg_0->target = target;
        arg_0->map = ovr033::PlayerMapPos(target);
        gbl::mapToBackGroundTile->drawTargetCursor = false;

        ovr033::redrawCombatArea(8, 3, gbl::mapToBackGroundTile->mapScreenTopLeft + Point::ScreenCenter);

        if (showRange)
        {
            if (TrySweepAttack(target, attacker) == true)
            {
                arg_4 = true;
                ovr025::clear_actions(attacker);
            }
            else
            {
                RecalcAttacksReceived(target, attacker);

                Item rangedWeapon = nullptr;

                if (ovr025::is_weapon_ranged(attacker) == true &&
                    ovr025::GetCurrentAttackItem(rangedWeapon, attacker) == true &&
                    ovr025::is_weapon_ranged_melee(attacker) == true &&
                    ovr025::getTargetRange(target, attacker) == 0)
                {
                    rangedWeapon = nullptr;
                }

                arg_4 = AttackTarget(rangedWeapon, 0, target, attacker);
            }
        }
    }
    else
    {
        arg_0->Clear();
    }

    return arg_4;
}

// static Set asc_41342 -- defined in decl/ovr014.h

bool ovr014::Target(DownedPlayerTile arg_0, bool allowTarget, bool canTargetEmptyGround, bool showRange, int maxRange, Player target, Player player01)
{
    Item dummyItem;

    arg_0->Clear();

    auto pos = ovr033::PlayerMapPos(target);

    char input_key = ' ';
    byte dir = 8;

    bool arg_4 = false;

    gbl::mapToBackGroundTile->drawTargetCursor = true;
    gbl::mapToBackGroundTile->size = 1;

    while (asc_41342->MemberOf(input_key) == false)
    {
        ovr033::redrawCombatArea(dir, 3, pos);
        pos = pos + gbl::MapDirectionDelta[dir]; // C#: pos += ...
        pos.MapBoundaryTrunc();

        int groundTile;
        int playerAtXY;

        ovr033::AtMapXY(groundTile, playerAtXY, pos);
        seg043::clear_keyboard();
        bool can_target = false;
        int range = 255;

        if (ovr032::canReachTarget(range, pos, ovr033::PlayerMapPos(player01)) == true)
        {
            can_target = true;

            if (showRange)
            {
                String range_text = String("Range = ") + ToString(range / 2) + "  ";

                seg041::displayString(range_text, 0, 10, 0x17, 0);
            }
        }
        else
        {
            if (showRange)
            {
                seg037::draw8x8_clear_area(0x17, 0x27, 0x17, 0);
            }
        }

        range /= 2;
        target = nullptr;

        if (can_target)
        {
            if (playerAtXY > 0)
            {
                target = gbl::player_array[playerAtXY];
            }
            else if (groundTile == gbl::Tile_DownPlayer)
            {
                auto c = gbl::downedPlayers.Find([&](const DownedPlayerTile& cell) { return cell->map == pos; });
                if (c != nullptr && c->target != nullptr)
                {
                    target = c->target;
                }
            }
        }

        if (target != nullptr)
        {
            gbl::display_hitpoints_ac = true;
            ovr025::CombatDisplayPlayerSummary(target);
        }
        else
        {
            seg037::draw8x8_clear_area(TextRegion::CombatSummary);
        }

        if (range > maxRange ||
            gbl::BackGroundTiles[groundTile]->move_cost == 0xff)
        {
            can_target = false;
        }

        if (target != nullptr)
        {
            if (CanSeeTargetA(target, player01) == false ||
                allowTarget == false)
            {
                can_target = false;
            }

            if (showRange)
            {
                if (player01 == target ||
                    (playerAtXY == 0 && groundTile == 0x1f))
                {
                    can_target = false;
                }
                else if (ovr025::is_weapon_ranged(player01) == true &&
                     (ovr025::GetCurrentAttackItem(dummyItem, player01) == false ||
                     (ovr025::BuildNearTargets(1, player01).Count() > 0 &&
                        ovr025::is_weapon_ranged_melee(player01) == false)))
                {
                    can_target = false;
                }
            }
        }
        else if (canTargetEmptyGround == false)
        {
            can_target = false;
        }

        String text = "Center Exit";

        if (can_target)
        {
            text = String("Target ") + text;
        }

        {   // goldbox-3ds: tell the host where the cursor is (display-only)
            auto tl = gbl::mapToBackGroundTile->mapScreenTopLeft;
            platform::AimShow(1, pos.x, pos.y, tl.x, tl.y);
        }
        input_key = ovr027::displayInput(false, 1, gbl::defaultMenuColors, text, "(Use Cursor keys) ");
        platform::AimHide();

        switch (input_key)
        {
            case '\r':
            case 'T':
                gbl::mapToBackGroundTile->drawTargetCursor = false;

                if (can_target)
                {
                    arg_0->map = pos;

                    if (target != nullptr)
                    {
                        arg_0->target = target;
                    }
                    else
                    {
                        arg_0->target = nullptr;
                    }

                    if (showRange)
                    {
                        arg_4 = sub_411D8(arg_0, showRange, arg_0->target, player01);
                    }
                    else
                    {
                        arg_4 = true;
                    }
                }

                if (can_target == false ||
                    arg_4 == false)
                {
                    ovr033::RedrawPosition(pos);
                    arg_4 = false;
                    arg_0->Clear();
                }
                break;

            case 'H':
                dir = 0;
                break;

            case 'I':
                dir = 1;
                break;

            case 'M':
                dir = 2;
                break;

            case 'Q':
                dir = 3;
                break;

            case 'P':
                dir = 4;
                break;

            case 'O':
                dir = 5;
                break;

            case 'K':
                dir = 6;
                break;

            case 'G':
                dir = 7;
                break;

            case '\0':
            case 'E':
                ovr033::RedrawPosition(pos);
                arg_0->Clear();
                arg_4 = false;
                break;

            case 'C':
                ovr033::redrawCombatArea(8, 0, pos);
                dir = 8;
                break;

            default:
                dir = 8;
                break;
        }
    }

    return arg_4;
}


Array<SortedCombatant> ovr014::copy_sorted_players(Player player) /* sub_4188F */
{
    auto scl = ovr032::Rebuild_SortedCombatantList(player, 0x7F, [](Player p) { return true; });

    return scl.ToArray();
}


Player ovr014::step_combat_list(bool arg_2, int step, int& list_index, Point& attackerPos, Array<SortedCombatant> sorted_list) /* sub_41932 */
{
    if (arg_2 == true)
    {
        attackerPos = sorted_list[list_index - 1]->pos;
    }
    else
    {
        ovr033::RedrawPosition(attackerPos);
    }

    list_index += step;

    if (list_index == 0)
    {
        list_index = sorted_list.Length(); // C#: sorted_list.GetLength(0)
    }

    if (list_index > sorted_list.Length()) // C#: sorted_list.GetLength(0)
    {
        list_index = 1;
    }

    Player newTarget = sorted_list[list_index - 1]->player;

    auto targetPos = sorted_list[list_index - 1]->pos;

    if (arg_2 == true)
    {
        ovr025::draw_missile_attack(0, 1, targetPos, attackerPos);
        attackerPos = targetPos;
    }

    return newTarget;
}

// static Set unk_41AE5, unk_41B05 -- defined in decl/ovr014.h


bool ovr014::aim_menu(DownedPlayerTile arg_0, bool allowTarget, bool canTargetEmptyGround, bool showRange, int maxRange, Player attacker) /* sub_41B25 */
{
    Player target; /* var_E5 */

    ovr025::load_missile_dax(false, 0, (Icon)0, 0x19);

    arg_0->Clear();

    bool arg_4 = false;

    if (maxRange == -1 || maxRange == 0xff)
    {
        if (attacker->activeItems->primaryWeapon() != nullptr)
        {
            maxRange = gbl::ItemDataTable[attacker->activeItems->primaryWeapon()->type]->range - 1;
        }
        else
        {
            maxRange = 1;
        }
    }

    if (maxRange == 0 ||
        maxRange == -1 || maxRange == 0xff)
    {
        maxRange = 1;
    }

    Array<SortedCombatant> sorted_list = copy_sorted_players(attacker);

    int list_index = 1;
    int next_prev_step = 0;
    int target_step = 0;

    Point attackerPos = Point();

    target = step_combat_list(true, next_prev_step, list_index, attackerPos, sorted_list);

    next_prev_step = 1;
    char input = ' ';

    while (arg_4 == false && unk_41AE5->MemberOf(input) == false)
    {
        if (CanSeeTargetA(target, attacker) == false)
        {
            target = step_combat_list(false, next_prev_step, list_index, attackerPos, sorted_list);
        }
        else
        {
            input = aim_sub_menu(allowTarget, showRange, maxRange, target, attacker);

            if (gbl::displayInput_specialKeyPressed == false)
            {
                switch (input)
                {
                    case 'N':
                        next_prev_step = 1;
                        target_step = 1;
                        break;

                    case 'P':
                        next_prev_step = -1;
                        target_step = -1;
                        break;

                    case 'M':
                    case 'H':
                    case 'K':
                    case 'G':
                    case 'O':
                    case 'Q':
                    case 'I':
                        arg_4 = Target(arg_0, allowTarget, canTargetEmptyGround, showRange, maxRange, target, attacker);
                        ovr025::load_missile_dax(false, 0, (Icon)0, 0x19);

                        sorted_list = copy_sorted_players(attacker);
                        target_step = 0;
                        break;

                    case 'T':
                        arg_4 = sub_411D8(arg_0, showRange, target, attacker);
                        ovr025::load_missile_dax(false, 0, (Icon)0, 0x19);

                        sorted_list = copy_sorted_players(attacker);
                        target_step = 0;
                        break;

                    case 'C':
                        ovr033::redrawCombatArea(8, 0, ovr033::PlayerMapPos(target));
                        target_step = 0;
                        break;
                }
            }
            else if (unk_41B05->MemberOf(input) == true)
            {
                arg_4 = Target(arg_0, allowTarget, canTargetEmptyGround, showRange, maxRange, target, attacker);
                ovr025::load_missile_dax(false, 0, (Icon)0, 0x19);
                sorted_list = copy_sorted_players(attacker);
                target_step = 0;
            }

            ovr033::RedrawPosition(ovr033::PlayerMapPos(target));

            target = step_combat_list((arg_4 == false && unk_41AE5->MemberOf(input) == false), target_step,
               list_index, attackerPos, sorted_list);
        }
    }

    if (showRange)
    {
        seg037::draw8x8_clear_area(0x17, 0x27, 0x17, 0);
    }

    return arg_4;
}


bool ovr014::find_target(bool clear_target, byte arg_2, int max_range, Player player) /* sub_41E44 */
{
    bool target_found = false;

    Player target = player->actions->target;

    if (clear_target == true ||
         (target != nullptr &&
           (target->combat_team == player->combat_team ||
            target->in_combat == false ||
            CanSeeTargetA(target, player) == false)))
    {
        player->actions->target = nullptr;
    }

    if (player->actions->target != nullptr)
    {
        target_found = true;
    }

    bool secondPass = false;
    bool var_5 = false;
    while (target_found == false && var_5 == false)
    {
        var_5 = secondPass;

        if (secondPass == true && clear_target == false)
        {
            gbl::mapToBackGroundTile->ignoreWalls = true;
        }

        int tryCount = 20;
        auto nearTargets = ovr025::BuildNearTargets(max_range, player);

        while (tryCount > 0 && target_found == false && nearTargets.Count() > 0)
        {
            tryCount--;
            int roll = ovr024::roll_dice(nearTargets.Count(), 1);

            auto epi = nearTargets[roll - 1];
            target = epi->player;

            if ((arg_2 != 0 && gbl::mapToBackGroundTile->ignoreWalls == true) ||
                CanSeeTargetA(target, player) == true)
            {
                target_found = true;
                player->actions->target = target;
            }
            else
            {
                nearTargets.Remove(epi);
            }
        }

        if (secondPass == false)
        {
            secondPass = true;
        }
    }

    gbl::mapToBackGroundTile->ignoreWalls = false;

    return target_found;
}


void ovr014::engulfs(Effect arg_0, object param, Player attacker)
{
    Player target = attacker->actions->target;

    if (gbl::bytes_1D2C9[1] == 2 &&
        target->in_combat == true &&
        target->HasAffect(Affects::clear_movement) == false &&
        target->HasAffect(Affects::reduce) == false)
    {
        target = attacker->actions->target;
        ovr025::DisplayPlayerStatusString(true, 12, String("engulfs ") + target->name, attacker);
        ovr024::add_affect(false, ovr033::GetPlayerIndex(target), 0, Affects::clear_movement, target);

        ovr013::CallAffectTable(Effect::Add, nullptr, target, Affects::clear_movement);
        ovr024::add_affect(false, ovr024::roll_dice(4, 2), 0, Affects::reduce, target);
        ovr024::add_affect(true, ovr033::GetPlayerIndex(target), 0, Affects::affect_8b, attacker);
    }
}


void ovr014::LoadMissleIconAndDraw(int icon_id, Player target, Player attacker) //sub_42159
{
    ovr025::load_missile_icons(icon_id + 13);

    ovr025::draw_missile_attack(0x1E, 1, ovr033::PlayerMapPos(target), ovr033::PlayerMapPos(attacker));
}


bool ovr014::sub_421C1(bool clear_target, int& range, Player player) // sub_421C1
{
    bool var_5 = true;

    if (find_target(clear_target, 0, 0xff, player) == true)
    {
        auto target = ovr033::PlayerMapPos(player->actions->target);

        if (ovr032::canReachTarget(range, target, ovr033::PlayerMapPos(player)) == true)
        {
            var_5 = false;
        }
    }

    return var_5;
}


void ovr014::attack_or_kill(Effect arg_0, object param, Player attacker)
{
    int range = 0xFF; /* simeon */

    byte attacksTired = 0;
    int attackTiresLeft = 4;

    attacker->actions->target = nullptr;
    sub_421C1(true, range, attacker);

    do
    {
        Player target = attacker->actions->target;

        range = ovr025::getTargetRange(target, attacker);
        attackTiresLeft--;

        if (target != nullptr)
        {
            if (range == 2 && (attacksTired & 1) == 0)
            {
                attacksTired |= 1;

                ovr025::DisplayPlayerStatusString(true, 10, "fires a disintegrate ray", attacker);
                LoadMissleIconAndDraw(5, target, attacker);

                if (ovr024::RollSavingThrow(0, SaveVerseType::BreathWeapon, target) == false)
                {
                    ovr024::KillPlayer("is disintergrated", Status::gone, target);
                }

                sub_421C1(false, range, attacker);
            }
            else if (range == 3 && (attacksTired & 2) == 0)
            {
                attacksTired |= 2;

                ovr025::DisplayPlayerStatusString(true, 10, "fires a stone to flesh ray", attacker);
                LoadMissleIconAndDraw(10, target, attacker);

                if (ovr024::RollSavingThrow(0, SaveVerseType::Petrification, target) == false)
                {
                    ovr024::KillPlayer("is Stoned", Status::stoned, target);
                }

                sub_421C1(false, range, attacker);
            }
            else if (range == 4 && (attacksTired & 4) == 0)
            {
                attacksTired |= 4;

                ovr025::DisplayPlayerStatusString(true, 10, "fires a death ray", attacker);
                LoadMissleIconAndDraw(5, target, attacker);

                if (ovr024::RollSavingThrow(0, (SaveVerseType)0, target) == false)
                {
                    ovr024::KillPlayer("is killed", Status::dead, target);
                }

                sub_421C1(false, range, attacker);
            }
            else if (range == 5 && (attacksTired & 8) == 0)
            {
                attacksTired |= 8;

                ovr025::DisplayPlayerStatusString(true, 10, "wounds you", attacker);
                LoadMissleIconAndDraw(5, target, attacker);

                ovr024::damage_person(false, (DamageOnSave)0, ovr024::roll_dice_save(8, 2) + 1, target);
                sub_421C1(false, range, attacker);
            }
            else if ((attacksTired & 0x10) == 0)
            {
                ovr023::sub_5D2E1(true, QuickFight::True, 0x54);
                attacksTired |= 0x10;
            }
            else if ((attacksTired & 0x20) == 0)
            {
                ovr023::sub_5D2E1(true, QuickFight::True, 0x37);
                attacksTired |= 0x20;
            }
            else if ((attacksTired & 0x40) == 0)
            {
                ovr023::sub_5D2E1(true, QuickFight::True, 0x15);
                attacksTired |= 0x40;
            }
        }
    } while (attackTiresLeft > 0 && attacker->actions->target != nullptr);
}


void ovr014::sub_425C6(Effect add_remove, object param, Player player)
{
    Affect affect = std::static_pointer_cast<Affect_>(param);

    gbl::spell_target = gbl::player_array[affect->affect_data];

    if (add_remove == Effect::Remove ||
        player->in_combat == false ||
        gbl::spell_target->in_combat == false)
    {
        ovr024::remove_affect(nullptr, Affects::clear_movement, gbl::spell_target);
        ovr024::remove_affect(nullptr, Affects::reduce, gbl::spell_target);

        if (add_remove == Effect::Add)
        {
            affect->callAffectTable = false;

            ovr024::remove_affect(affect, Affects::affect_8b, player);
        }
    }
    else
    {
        player->attack1_AttacksLeft = 2;
        player->attack2_AttacksLeft = 0;
        player->attack1_DiceCount = 2;
        player->attack1_DiceSize = 8;

        AttackTarget(nullptr, 1, gbl::spell_target, player);

        ovr025::clear_actions(player);

        if (gbl::spell_target->in_combat == false)
        {
            ovr024::remove_affect(nullptr, Affects::affect_8b, player);
            ovr024::remove_affect(nullptr, Affects::clear_movement, gbl::spell_target);
            ovr024::remove_affect(nullptr, Affects::reduce, gbl::spell_target);
        }
    }
}


void ovr014::AffectOwlbearHugRoundAttack(Effect arg_0, object param, Player player) // sub_426FC
{
    Affect affect = std::static_pointer_cast<Affect_>(param);

    gbl::spell_target = gbl::player_array[affect->affect_data];

    if (arg_0 == Effect::Remove ||
        player->in_combat == false ||
        gbl::spell_target->in_combat == false)
    {
        ovr024::remove_affect(nullptr, Affects::clear_movement, gbl::spell_target);
        if (arg_0 == Effect::Add)
        {
            affect->callAffectTable = false;
            ovr024::remove_affect(affect, Affects::owlbear_hug_round_attack, player);
        }
    }
    else
    {
        player->attack1_AttacksLeft = 1;
        player->attack2_AttacksLeft = 0;
        player->attack1_DiceCount = 2;
        player->attack1_DiceSize = 8;


        AttackTarget(nullptr, 2, gbl::spell_target, player);

        ovr025::clear_actions(player);

        if (gbl::spell_target->in_combat == false)
        {
            ovr024::remove_affect(nullptr, Affects::owlbear_hug_round_attack, player);
            ovr024::remove_affect(nullptr, Affects::clear_movement, gbl::spell_target);
        }
    }
}


void ovr014::AffectOwlbearHugAttackCheck(Effect arg_0, object param, Player player) // hugs
{
    if (gbl::attack_roll >= 18)
    {
        gbl::spell_target = player->actions->target;
        ovr025::DisplayPlayerStatusString(true, 12, String("hugs ") + gbl::spell_target->name, player);

        ovr024::add_affect(false, ovr033::GetPlayerIndex(gbl::spell_target), 0, Affects::clear_movement, gbl::spell_target);
        ovr013::CallAffectTable(Effect::Add, nullptr, gbl::spell_target, Affects::clear_movement);

        ovr024::add_affect(true, ovr033::GetPlayerIndex(gbl::spell_target), 0, Affects::owlbear_hug_round_attack, player);
    }
}


bool ovr014::god_intervene()
{
    bool intervened = false;

    if (Cheats::allow_gods_intervene)
    {
        intervened = true;
        ovr025::string_print01("The Gods intervene!");

        for (Player player : gbl::TeamList)
        {
            if (player->combat_team == CombatTeam::Enemy)
            {
                player->in_combat = false;
                player->health_status = Status::dead;

                gbl::CombatMap[ovr033::GetPlayerIndex(player)]->size = 0;
            }

            ovr025::clear_actions(player);
        }

        ovr033::redrawCombatArea(8, 0xff, gbl::mapToBackGroundTile->mapScreenTopLeft + Point::ScreenCenter);
    }

    return intervened;
}

}  // namespace engine
