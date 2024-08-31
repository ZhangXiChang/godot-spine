#pragma once
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/skeleton2d.hpp>
#include <godot_cpp/classes/bone2d.hpp>
#include <godot_cpp/classes/polygon2d.hpp>
#include <godot_cpp/classes/resource_loader.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

#include <spine/spine.h>

using namespace spine;

class SpineImporter : public Object
{
    GDCLASS(SpineImporter, Object)
private:
    std::shared_ptr<Atlas> atlas;
    std::shared_ptr<SkeletonData> skeleton_data;
    Ref<Texture2D> texture;

public:
    static void _bind_methods()
    {
        ClassDB::bind_method(D_METHOD("load", "atlas_path", "skel_path", "texture_path"), &SpineImporter::load);
        ClassDB::bind_method(D_METHOD("to_node2d", "node_name"), &SpineImporter::to_node2d);
    }

    Error load(godot::String atlas_path, godot::String skel_path, godot::String texture_path)
    {
        this->atlas = std::make_shared<Atlas>(atlas_path.utf8().get_data(), nullptr);
        if (!this->atlas)
        {
            return FAILED;
        }
        this->skeleton_data = std::shared_ptr<SkeletonData>(SkeletonBinary(this->atlas.get()).readSkeletonDataFile(skel_path.utf8().get_data()));
        if (!this->skeleton_data)
        {
            return FAILED;
        }
        this->texture = ResourceLoader::get_singleton()->load(texture_path);
        return OK;
    }
    Node2D *to_node2d(godot::String node_name)
    {
        auto spine_skeleton = Skeleton(this->skeleton_data.get());
        spine_skeleton.setScaleY(-1);
        spine_skeleton.updateWorldTransform();

        auto spine_node = memnew(Node2D);
        spine_node->set_name(node_name);

        auto skeleton = memnew(Skeleton2D);
        spine_node->add_child(skeleton);
        skeleton->set_owner(spine_node);
        skeleton->set_name("Skeleton");
        skeleton->set_visible(false);
        parse_spine_bone(spine_node, skeleton, spine_skeleton.getRootBone());

        auto skin = memnew(Node2D);
        spine_node->add_child(skin);
        skin->set_owner(spine_node);
        skin->set_name("Skin");
        for (size_t i = 0; i < spine_skeleton.getDrawOrder().size(); i++)
        {
            auto slot = spine_skeleton.getDrawOrder()[i];
            auto attachment = slot->getAttachment();
            if (attachment && attachment->getRTTI().isExactly(MeshAttachment::rtti))
            {
                auto mesh = (MeshAttachment *)attachment;

                auto polygon = memnew(Polygon2D);
                skin->add_child(polygon);
                polygon->set_owner(spine_node);
                polygon->set_name(mesh->getName().buffer());
                polygon->set_skeleton(polygon->get_path_to(skeleton));

                polygon->set_texture(this->texture);
                auto polygon_uv = PackedVector2Array();
                for (size_t i = 0; i < mesh->getHullLength(); i += 2)
                {
                    polygon_uv.append(Vector2(mesh->getUVs()[i] * this->texture->get_width(), mesh->getUVs()[i + 1] * this->texture->get_height()));
                }
                polygon->set_uv(polygon_uv);

                auto polygon_world_verts = PackedVector2Array();
                spine::Vector<float> mesh_world_verts;
                mesh_world_verts.setSize(mesh->getWorldVerticesLength(), 0);
                mesh->computeWorldVertices(*slot, mesh_world_verts);
                for (size_t i = 0; i < mesh->getHullLength(); i += 2)
                {
                    polygon_world_verts.append(Vector2(mesh_world_verts[i], mesh_world_verts[i + 1]));
                }
                polygon->set_polygon(polygon_world_verts);
            }
        }
        return spine_node;
    }
    void parse_spine_bone(Node *owner, Node2D *base_node, Bone *spine_bone)
    {
        auto bone = memnew(Bone2D);
        base_node->add_child(bone);
        bone->set_owner(owner);
        bone->set_name(spine_bone->getData().getName().buffer());
        bone->set_global_position(Vector2(spine_bone->getWorldX(), spine_bone->getWorldY()));
        bone->set_global_scale(Vector2(spine_bone->getWorldScaleX(), spine_bone->getWorldScaleY()));
        if (spine_bone->getChildren().size() == 0)
        {
            bone->set_autocalculate_length_and_angle(false);
        }
        for (size_t i = 0; i < spine_bone->getChildren().size(); i++)
        {
            parse_spine_bone(owner, bone, spine_bone->getChildren()[i]);
        }
    }
};
