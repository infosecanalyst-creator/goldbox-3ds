#include "engine/engine.h"

namespace engine {

ovr032::MapReach_::MapReach_(bool r, int ra, Point p)
{
    reach = r; target = p; range = ra;
}

// static MapReach[, ,] mapReachCache = new MapReach[Point.MapMaxY * Point.MapMaxX, Point.MapMaxY * Point.MapMaxX, 2];
// (defined in decl/ovr032.h)

//internal static void buildMapCache()
//{
//    for (int y1 = 0; y1 < Point.MapMaxY; y1++)
//    {
//        for (int x1 = 0; x1 < Point.MapMaxX; x1++)
//        {
//            var p1 = new Point(x1,y1);
//            for (int y2 = 0; y2 < Point.MapMaxY; y2++)
//            {
//                for (int x2 = 0; x2 < Point.MapMaxX; x2++)
//                {
//                    gbl.mapToBackGroundTile.field_6 = false;
//                    Point p2 = new Point(x2, y2);
//                    int range = -1;
//                    bool reach = canReachTargetCalc(gbl.mapToBackGroundTile, ref range, ref p2, p1);
//
//                    mapReachCache[(y1 * Point.MapMaxX) + x1, (y2 * Point.MapMaxX) + x2, 0] = new MapReach(reach, range, p2);
//
//                    gbl.mapToBackGroundTile.field_6 = true;
//                    p2 = new Point(x2, y2);
//                    range = -1;
//                    reach = canReachTargetCalc(gbl.mapToBackGroundTile, ref range, ref p2, p1);
//
//                    mapReachCache[(y1 * Point.MapMaxX) + x1, (y2 * Point.MapMaxX) + x2, 1] = new MapReach(reach, range, p2);
//                }
//            }
//        }
//    }
//}

ovr032::MapReach ovr032::MapCacheGet(Point p1, Point p2, bool ignoreWalls)
{
    int nIgnoreWalls = ignoreWalls ? 1 : 0;
    MapReach mr = mapReachCache((p2.y * Point::MapMaxX) + p2.x, (p1.y * Point::MapMaxX) + p1.x, nIgnoreWalls);
    if (mr != nullptr) return mr;

    bool bkup_val = gbl::mapToBackGroundTile->ignoreWalls;
    gbl::mapToBackGroundTile->ignoreWalls = ignoreWalls;

    mr = canReachTargetCalc(gbl::mapToBackGroundTile, p2, p1);

    gbl::mapToBackGroundTile->ignoreWalls = bkup_val;

    mapReachCache((p2.y * Point::MapMaxX) + p2.x, (p1.y * Point::MapMaxX) + p1.x, nIgnoreWalls) = mr;
    return mr;
}

void ovr032::canReachTarget(Point& target, Point attacker)
{
    MapReach mr = MapCacheGet(attacker, target, gbl::mapToBackGroundTile->ignoreWalls);

    target = Point(mr->target);
}

bool ovr032::canReachTarget(int& range, Point target, Point attacker)
{
    MapReach mr = MapCacheGet(attacker, target, gbl::mapToBackGroundTile->ignoreWalls);

    if (mr->range > (range * 2) + 1)
    {
        return false;
    }
    else
    {
        range = mr->range;
        return mr->reach;
    }
}

ovr032::MapReach ovr032::canReachTargetCalc(Struct_1D1BC groundTilesMap, Point outPos, Point attacker) /* sub_733F1 */
{
    SteppingPath var_31 = New<SteppingPath_>();
    SteppingPath var_19 = New<SteppingPath_>();

    int max_range = (256 * 2) + 1;
    var_19->attacker = attacker;
    var_19->target = outPos;

    var_19->CalculateDeltas();

    var_31->attacker.x = 0;
    var_31->attacker.y = gbl::BackGroundTiles[groundTilesMap[attacker]]->field_1;

    if (var_19->diff_x > var_19->diff_y)
    {
        var_31->target.x = var_19->diff_x;
    }
    else
    {
        var_31->target.x = var_19->diff_y;
    }

    var_31->target.y = gbl::BackGroundTiles[groundTilesMap[attacker]]->field_1;
    var_31->CalculateDeltas();
    bool finished = false;

    do
    {
        int gt = groundTilesMap[var_19->current];
        Struct_189B4 s189 = gbl::BackGroundTiles[gt];

        if (groundTilesMap->ignoreWalls == false && s189->field_2 > var_31->current.y)
        {
            return New<MapReach_>(false, var_19->steps, var_19->current);
        }

        // range is for cache hard coded to 256, thus max_range = 513, so skip this.
        if (var_19->steps > max_range)
        {
            return New<MapReach_>(false, var_19->steps, var_19->current);
        }

        var_31->Step();
        finished = !var_19->Step();
    } while (finished == false);

    return New<MapReach_>(true, var_19->steps, outPos);
}

/// <summary>
/// Returns if playerB can see playerA
/// </summary>
bool ovr032::CanSeeCombatant(int direction, Point playerA, Point playerB) /* sub_7354A */
{
    if (playerA.MapInBounds() == false || playerB.MapInBounds() == false)
    {
        return false;
    }

    if (direction == 0xff || direction == 8)
    {
        return true;
    }

    int facingX = playerB.x + gbl::MapDirectionXDelta[direction];
    int facingY = playerB.y + gbl::MapDirectionYDelta[direction];

    if (playerB == playerA ||
        (facingX == playerA.x && facingY == playerA.y))
    {
        return true;
    }

    bool canSee;

    switch (direction)
    {
        case 0:
            canSee = ((playerA.x >= facingX && playerA.y <= ((facingX - playerA.x) + facingY)) ||
                (playerA.x <= facingX && playerA.y <= ((playerA.x - facingX) + facingY)));
            break;

        case 1:
            canSee = ((playerA.x >= facingX && playerA.y <= ((facingX - playerA.x) + facingY)) ||
                (playerA.x >= ((facingX - facingY) + playerA.y) && playerA.y <= facingY));
            break;

        case 2:
            canSee = ((playerA.x >= (facingX + facingY - playerA.y) && playerA.y <= facingY) ||
                (playerA.x >= (facingX + playerA.y - facingY) && playerA.y >= facingY));
            break;

        case 3:
            canSee = ((playerA.x >= ((facingX + playerA.y) - facingY) && playerA.y >= facingY) ||
                (playerA.x >= facingX && playerA.y >= ((playerA.x - facingX) + facingY)));
            break;

        case 4:
            canSee = ((playerA.x >= facingX && playerA.y >= ((playerA.x - facingX) + facingY)) ||
                (playerA.x <= facingX && playerA.y >= ((facingX - playerA.x) + facingY)));
            break;

        case 5:
            canSee = ((playerA.x <= facingX && playerA.y >= ((facingX - playerA.x) + facingY)) ||
                (playerA.x <= ((facingX + facingY) - playerA.y) && playerA.y >= facingY));
            break;

        case 6:
            canSee = ((playerA.x <= ((facingX + facingY) - playerA.y) && playerA.y >= facingY) ||
                (playerA.x <= ((facingX + playerA.y) - facingY) && playerA.y <= facingY));
            break;

        case 7:
            canSee = ((playerA.x <= ((facingX + playerA.y) - facingY) && playerA.y <= facingY) ||
                (playerA.x <= facingX && playerA.y <= ((playerA.x - facingX) + facingY)));
            break;

        case 8:
            canSee = true;
            break;

        default:
            throw ApplicationException("Switch value unexpected");
    }

    return canSee;
}

List<SortedCombatant> ovr032::Rebuild_SortedCombatantList(Player player, int max_range, std::function<bool(Player)> filter) /* sub_738D8 */
{
    auto cm = gbl::CombatMap[ovr033::GetPlayerIndex(player)];

    return Rebuild_SortedCombatantList(cm->size, max_range, cm->pos, filter);
}

List<SortedCombatant> ovr032::Rebuild_SortedCombatantList(int size, int max_range, Point pos, std::function<bool(Player)> filter) /* sub_738D8 */
{
    auto deltas = ovr033::GetSizeBasedMapDeltas(size);
    auto attackerMap = ovr033::BuildSizeMap(size, pos);

    auto sortedCombatants = List<SortedCombatant>();

    for (int playerIndex = 1; playerIndex <= gbl::CombatantCount; playerIndex++)
    {
        auto combatantMap = gbl::CombatMap[playerIndex];
        if (combatantMap->size > 0 && filter(gbl::player_array[playerIndex]))
        {
            auto targetMap = ovr033::BuildSizeMap(combatantMap->size, combatantMap->pos);

            bool found = false;
            int found_range = max_range;
            Point found_target = Point();
            Point found_attacker = Point();

            for (auto targetPos : targetMap)
            {
                for (auto attackerPos : attackerMap)
                {
                    int tmp_range = max_range;

                    if (canReachTarget(tmp_range, targetPos, attackerPos) == true)
                    {
                        found = true;

                        if (tmp_range < found_range)
                        {
                            found_range = tmp_range;
                            found_attacker = attackerPos;
                            found_target = targetPos;
                        }
                    }
                }
            }

            if (found == true)
            {
                byte tmpDir = FindCombatantDirection(found_target, found_attacker);

                auto combatant = New<SortedCombatant_>(gbl::player_array[playerIndex], combatantMap->pos, found_range, tmpDir);
                sortedCombatants.Add(combatant);
            }
        }
    }

    // PORT-NOTE: C# List.Sort is an unstable introsort; cs::List::Sort is a stable
    // sort. Combatants that compare equal may come out in a different order.
    sortedCombatants.Sort();

    return sortedCombatants;
}


byte ovr032::FindCombatantDirection(Point target, Point attacker)
{
    byte dir = 0;

    while (CanSeeCombatant(dir, target, attacker) == false && dir < 8)
    {
        dir++;
    }

    return dir;
}


}  // namespace engine
