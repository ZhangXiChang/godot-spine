#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

#include "godot-spine/spine-editor-plugin.h"

void register_initializer(ModuleInitializationLevel p_level)
{
    switch (p_level)
    {
    case MODULE_INITIALIZATION_LEVEL_SCENE:
        break;
    case MODULE_INITIALIZATION_LEVEL_EDITOR:
        ClassDB::register_internal_class<SpineEditorPlugin>();
        EditorPlugins::add_by_type<SpineEditorPlugin>();
        break;
    }
}

extern "C"
{
    GDExtensionBool GDE_EXPORT entry(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization)
    {
        GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);
        init_obj.register_initializer(register_initializer);
        return init_obj.init();
    }
}
