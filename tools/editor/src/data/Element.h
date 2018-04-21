#include <data/Element.h>
#include "Main.h"


GB_EDITOR_DATA_NS_BEGIN
struct ElementType
{
    static constexpr std::uint32_t Main = 0xf0;
};

class ElementRecipe
{
    GB_SINGLETON(ElementRecipe);

public:
    inline void Reg()
	{
	    GB_RENDER_DATA_ELEMENT_RECIPE_REG(ElementType::Main, Main);
	}
};

GB_EDITOR_DATA_NS_END
