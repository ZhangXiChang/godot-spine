#pragma once
#include <godot_cpp/classes/editor_plugin.hpp>
// #include <godot_cpp/classes/editor_import_plugin.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/skeleton2d.hpp>
#include <godot_cpp/classes/bone2d.hpp>
#include <godot_cpp/classes/polygon2d.hpp>
// #include <godot_cpp/variant/utility_functions.hpp>

// class SpineImporter : public EditorImportPlugin
// {
//     GDCLASS(SpineImporter, EditorImportPlugin)
// public:
//     static void _bind_methods() {}

//     String _get_importer_name() const override
//     {
//         return "SpineImporter";
//     }
//     String _get_visible_name() const override
//     {
//         return "ToPackedScene";
//     }
//     PackedStringArray _get_recognized_extensions() const override
//     {
//         return {"atlas", "skel"};
//     }
//     int32_t _get_preset_count() const override
//     {
//         return 1;
//     }
//     String _get_preset_name(int32_t p_preset_index) const override
//     {
//         return "Default";
//     }
//     TypedArray<Dictionary> _get_import_options(const String &p_path, int32_t p_preset_index) const override
//     {
//         switch (p_preset_index)
//         {
//         case 0:
//             Dictionary option;
//             option["name"] = "use_red_anyway";
//             option["default_value"] = false;
//             // option["property_hint"] = PROPERTY_HINT_FILE;
//             // option["hint_string"] = "asset_path or skel_path";
//             // option["usage"] = PROPERTY_USAGE_DEFAULT;
//             return {option};
//         }
//         return {};
//     }
//     bool _get_option_visibility(const String &p_path, const StringName &p_option_name, const Dictionary &p_options) const override
//     {
//         return true;
//     }
//     String _get_save_extension() const override
//     {
//         return "tscn";
//     }
//     String _get_resource_type() const override
//     {
//         return "PackedScene";
//     }
//     Error _import(const String &p_source_file, const String &p_save_path, const Dictionary &p_options, const TypedArray<String> &p_platform_variants, const TypedArray<String> &p_gen_files) const override
//     {
//         auto file = FileAccess::open(p_source_file, FileAccess::READ);
//         if (file.is_null())
//         {
//             return Error::FAILED;
//         }
//         // auto skeleton = new Skeleton2D;
//         // auto skin = new Node2D;
//         // auto spine_node = new Node2D;
//         // spine_node->add_child(skeleton);
//         // spine_node->add_child(skin);
//         // Ref<PackedScene> spine_scene;
//         // spine_scene.instantiate();
//         // spine_scene->pack(spine_node);
//         // delete spine_node;
//         // ResourceSaver::get_singleton()->save(spine_scene, p_save_path.path_join("." + this->_get_save_extension()));
//         return Error::OK;
//     }
//     double _get_priority() const override
//     {
//         return 1.;
//     }
//     int32_t _get_import_order() const override
//     {
//         return 0;
//     }
// };

class SpineEditorPlugin : public EditorPlugin
{
    GDCLASS(SpineEditorPlugin, EditorPlugin)
private:
public:
    static void _bind_methods() {}

    void _enter_tree() override
    {
    }
    void _exit_tree() override
    {
    }
};
