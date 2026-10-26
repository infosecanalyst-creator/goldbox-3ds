// engine/decl/ovr007.h - declarations for engine/ovr007.cs.
// Fragment: included by engine/engine.h inside namespace engine. Owned by the
// translator of engine/ovr007.cpp.
// ---------------------------------------------------------------------------
// ovr007.cs
// ---------------------------------------------------------------------------
struct ovr007 {
    static char ShopChooseItem(int& index, Item& selectedItem); // sub_2F04E
    static int ItemsValue(Item item_ptr);
    static bool PlayerAddItem(Item item); /*was overloaded */
    static void shop_buy(); /* sub_2F474 */
    static void CityShop(); // sub_2F6E7
};

