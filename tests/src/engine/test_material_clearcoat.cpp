#include <donut/engine/SceneTypes.h>
#include <donut/tests/utils.h>
#include <cstddef>

using namespace donut::math;
#include <donut/shaders/material_cb.h>

// Keep the material stride and the byte offsets used by LoadMaterialConstants.
static_assert(sizeof(MaterialConstants) == 208);
static_assert(offsetof(MaterialConstants, clearcoatFactor) == 116);
static_assert(offsetof(MaterialConstants, clearcoatRoughness) == 120);

int main()
{
    donut::engine::Material material;
    MaterialConstants constants{};
    material.FillConstantBuffer(constants);
    CHECK(constants.clearcoatFactor == 0.f);
    CHECK(constants.clearcoatRoughness == 0.f);

    CHECK(material.SetProperty("clearcoatFactor", float4(0.75f)));
    CHECK(material.SetProperty("clearcoatRoughness", float4(0.2f)));
    material.FillConstantBuffer(constants, true);
    CHECK(constants.clearcoatFactor == 0.75f);
    CHECK(constants.clearcoatRoughness == 0.2f);
    CHECK(constants.ior == material.ior);
    return 0;
}
