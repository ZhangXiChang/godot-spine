#pragma once
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/skeleton2d.hpp>
#include <godot_cpp/classes/bone2d.hpp>
#include <godot_cpp/classes/polygon2d.hpp>

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
        delete this->animation_state_data;
    }

private:
    Atlas *atlas = nullptr;
    SkeletonData *skeleton_data = nullptr;
    AnimationStateData *animation_state_data = nullptr;

public:
    static void _bind_methods()
    {
        ClassDB::bind_method(D_METHOD("load", "atlas_path", "skel_path"), &SpineImporter::load);
        ClassDB::bind_method(D_METHOD("to_node2d", "node_name"), &SpineImporter::to_node2d);
    }

    Error load(godot::String atlas_path, godot::String skel_path)
    {
        this->atlas = new Atlas(atlas_path.utf8().get_data(), nullptr);
        if (this->atlas == nullptr)
        {
            return FAILED;
        }
        auto skeleton_format = SkeletonBinary(this->atlas);
        this->skeleton_data = skeleton_format.readSkeletonDataFile(skel_path.utf8().get_data());
        if (this->skeleton_data == nullptr)
        {
            return FAILED;
        }
        this->animation_state_data = new AnimationStateData(this->skeleton_data);
        return OK;
    }
    Node2D *to_node2d(godot::String node_name)
    {
        auto skeleton = Skeleton(this->skeleton_data);
        skeleton.setScaleY(-1.);
        auto animation_state = AnimationState(this->animation_state_data);
        animation_state.setEmptyAnimations(0);
        animation_state.update(0);
        animation_state.apply(skeleton);
        skeleton.setToSetupPose();
        skeleton.updateWorldTransform();

        auto spine_node = memnew(Node2D);
        spine_node->set_name(node_name);

        auto skeleton2d = memnew(Skeleton2D);
        skeleton2d->set_name("Skeleton");
        spine_node->add_child(skeleton2d);
        skeleton2d->set_owner(spine_node);
        auto root_bone = memnew(Bone2D);
        root_bone->set_name(skeleton.getRootBone()->getData().getName().buffer());
        skeleton2d->add_child(root_bone);
        root_bone->set_owner(spine_node);
        root_bone->set_position({skeleton.getRootBone()->getX(), skeleton.getRootBone()->getY()});
        root_bone->set_rotation(skeleton.getRootBone()->getRotation());
        parse_spine_bone(spine_node, root_bone, skeleton.getRootBone()->getChildren());

        auto skin = memnew(Node2D);
        skin->set_name("Skin");
        spine_node->add_child(skin);
        skin->set_owner(spine_node);
        parse_spine_skin(spine_node);
        return spine_node;
    }
    void parse_spine_bone(Node *owner, Bone2D *base_bone, spine::Vector<Bone *> &spine_child_bones)
    {
        for (size_t i = 0; i < spine_child_bones.size(); i++)
        {
            if (spine_child_bones[i]->getData().getName() == "F_Hip")
            {
                UtilityFunctions::print(Vector2({spine_child_bones[i]->getX(), spine_child_bones[i]->getY()}));
            }
            auto bone = memnew(Bone2D);
            bone->set_name(spine_child_bones[i]->getData().getName().buffer());
            bone->set_position({spine_child_bones[i]->getX(), spine_child_bones[i]->getY()});
            bone->set_rotation(spine_child_bones[i]->getRotation());
            base_bone->add_child(bone);
            bone->set_owner(owner);
            parse_spine_bone(owner, bone, spine_child_bones[i]->getChildren());
            if (bone->get_child_count() == 0)
            {
                bone->set_autocalculate_length_and_angle(false);
            }
        }
    }
    void parse_spine_skin(Node *owner) {}
};
