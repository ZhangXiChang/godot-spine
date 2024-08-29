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
public:
    ~SpineImporter()
    {
        delete this->atlas;
        delete this->skeleton_data;
    }

private:
    Atlas *atlas = nullptr;
    SkeletonData *skeleton_data = nullptr;
    Ref<Texture2D> texture;

public:
    static void _bind_methods()
    {
        ClassDB::bind_method(D_METHOD("load", "atlas_path", "skel_path", "texture_path"), &SpineImporter::load);
        ClassDB::bind_method(D_METHOD("to_node2d", "node_name"), &SpineImporter::to_node2d);
    }

    Error load(godot::String atlas_path, godot::String skel_path, godot::String texture_path)
    {
        this->atlas = new Atlas(atlas_path.utf8().get_data(), nullptr);
        if (!this->atlas)
        {
            return FAILED;
        }
        this->skeleton_data = SkeletonBinary(this->atlas).readSkeletonDataFile(skel_path.utf8().get_data());
        if (!this->skeleton_data)
        {
            return FAILED;
        }
        auto root_bone_data = this->skeleton_data->getBones()[0];
        root_bone_data->setScaleX(1.f);
        root_bone_data->setScaleY(-1.f);
        this->texture = ResourceLoader::get_singleton()->load(texture_path);
        return OK;
    }
    Node2D *to_node2d(godot::String node_name)
    {
        auto spine_skeleton = Skeleton(this->skeleton_data);

        auto spine_node = memnew(Node2D);
        spine_node->set_name(node_name);

        auto skeleton = memnew(Skeleton2D);
        skeleton->set_name("Skeleton");
        spine_node->add_child(skeleton);
        skeleton->set_owner(spine_node);
        auto root_bone = memnew(Bone2D);
        auto spine_root_bone = spine_skeleton.getRootBone();
        root_bone->set_name(spine_root_bone->getData().getName().buffer());
        skeleton->add_child(root_bone);
        root_bone->set_owner(spine_node);
        root_bone->set_position(Vector2(spine_root_bone->getX(), spine_root_bone->getY()));
        root_bone->set_rotation(Math::deg_to_rad(spine_root_bone->getRotation()));
        root_bone->set_scale(Vector2(spine_root_bone->getScaleX(), spine_root_bone->getScaleY()));
        parse_spine_bone(spine_node, root_bone, spine_root_bone->getChildren());

        auto skin = memnew(Node2D);
        skin->set_name("Skin");
        spine_node->add_child(skin);
        skin->set_owner(spine_node);
        for (size_t i = 0; i < spine_skeleton.getDrawOrder().size(); i++)
        {
            auto slot = spine_skeleton.getDrawOrder()[i];
            auto attachment = slot->getAttachment();
            if (attachment)
            {
                if (attachment->getRTTI().isExactly(MeshAttachment::rtti))
                {
                    auto mesh = (MeshAttachment *)attachment;
                    auto polygon = memnew(Polygon2D);
                    polygon->set_texture(this->texture);
                    polygon->set_name(slot->getData().getName().buffer());
                    polygon->set_position(Vector2(slot->getBone().getX(), slot->getBone().getY()));
                    polygon->set_rotation(Math::deg_to_rad(slot->getBone().getRotation()));
                    polygon->set_scale(Vector2(slot->getBone().getScaleX(), slot->getBone().getScaleY()));
                    skin->add_child(polygon);
                    polygon->set_owner(spine_node);
                    auto uv = PackedVector2Array();
                    for (size_t i = 0; i < mesh->getUVs().size(); i += 2)
                    {
                        auto u = mesh->getUVs()[i] * this->texture->get_width();
                        auto v = mesh->getUVs()[i + 1] * this->texture->get_height();
                        uv.push_back(Vector2(u, v));
                    }
                    polygon->set_polygon(uv);
                    polygon->set_internal_vertex_count((mesh->getUVs().size() / 2) - (mesh->getHullLength() / 2));
                }
            }
        }
        return spine_node;
    }
    void parse_spine_bone(Node *owner, Bone2D *base_bone, spine::Vector<Bone *> &spine_child_bones)
    {
        for (size_t i = 0; i < spine_child_bones.size(); i++)
        {
            auto spine_bone = spine_child_bones[i];
            auto bone = memnew(Bone2D);
            bone->set_name(spine_bone->getData().getName().buffer());
            bone->set_position(Vector2(spine_bone->getX(), spine_bone->getY()));
            bone->set_rotation(Math::deg_to_rad(spine_bone->getRotation()));
            bone->set_scale(Vector2(spine_bone->getScaleX(), spine_bone->getScaleY()));
            base_bone->add_child(bone);
            bone->set_owner(owner);
            parse_spine_bone(owner, bone, spine_bone->getChildren());
            if (bone->get_child_count() == 0)
            {
                bone->set_autocalculate_length_and_angle(false);
            }
        }
    }
};
