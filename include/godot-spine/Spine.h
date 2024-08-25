#pragma once
#include <godot_cpp/classes/node2d.hpp>

class Spine : public Node2D
{
    GDCLASS(Spine, Node2D)
protected:
    static void _bind_methods()
    {
        ClassDB::bind_method(D_METHOD("get_atlas_path"), &Spine::get_atlas_path);
        ClassDB::bind_method(D_METHOD("set_atlas_path", "atlas_path"), &Spine::set_atlas_path);
        ClassDB::add_property("Spine", PropertyInfo(Variant::STRING, "atlas_path", PROPERTY_HINT_FILE, ".atlas"), "set_atlas_path", "get_atlas_path");

        ClassDB::bind_method(D_METHOD("get_skel_path"), &Spine::get_skel_path);
        ClassDB::bind_method(D_METHOD("set_skel_path", "skel_path"), &Spine::set_skel_path);
        ClassDB::add_property("Spine", PropertyInfo(Variant::STRING, "skel_path", PROPERTY_HINT_FILE, ".skel"), "set_skel_path", "get_skel_path");
    }

private:
    godot::String atlas_path;
    godot::String get_atlas_path() { return this->atlas_path; }
    void set_atlas_path(godot::String atlas_path) { this->atlas_path = atlas_path; }

    godot::String skel_path;
    godot::String get_skel_path() { return this->skel_path; }
    void set_skel_path(godot::String skel_path) { this->skel_path = skel_path; }
};
