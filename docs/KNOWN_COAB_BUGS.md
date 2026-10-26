# Known coab bugs (kept on purpose for now)

The C++ port reproduces coab exactly so it can be diffed against it. These are
bugs in coab itself that the agents flagged while translating. Once the port is
verified, a `--fix-coab-bugs` mode (or per-bug flags) can correct them.

| Where | Bug |
|---|---|
| ovr032 `mapReachCache` | Line-of-sight/reach cache is never cleared, so results from one battle map leak into later battles. |
| ovr031 `MapCoordIsValid` | Checks `mapX >= 0` twice and never `mapY >= 0`. |
| ovr031 `Draw3dWorldBackground` | Calls `get_wall_x2(mapPosY, mapPosY)`; second argument should probably be `mapPosX`. |
| ovr003 `SetupCommandTable` | ECL opcode 0x1F is registered with a null handler (crash if executed). |
| ovr020 `ready_Item` | Sets `result = Weld.Ok` after all checks, so readying always succeeds. |
| ovr023 `SpellDispelMagic` | Reads `affect.type` while `affect` is null. |
| ovr023 `SpellRestoration` | Can index `ClassLevel[30]` if no class qualifies. |
| ovr017 `BuildLoadablePlayersLists` | Indexes the full save path instead of the file name. |
| Player(data, offset) | Reads `spellCastCount` from `0x12d + j + i*i`, ignoring `offset`. |
| Area1 `field_18C` | Declared at offset 0x18E, same as `time_minutes_ones`. |
| Set `SetRange` | Infinite loop if the upper bound is 255. |
