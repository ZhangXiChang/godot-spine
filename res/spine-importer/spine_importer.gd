@tool
extends Control

var editor_file_dialog = EditorFileDialog.new()

func _enter_tree() -> void:
	#文件选择弹窗
	editor_file_dialog.access = EditorFileDialog.ACCESS_FILESYSTEM
	editor_file_dialog.transient = true
	editor_file_dialog.transient_to_focused = true
	editor_file_dialog.file_selected.connect(file_selected)
	get_tree().root.add_child(editor_file_dialog)

func _on_browse_atlas_file_pressed() -> void:
	editor_file_dialog.file_mode = EditorFileDialog.FILE_MODE_OPEN_FILE
	editor_file_dialog.filters = ["*.atlas;图集"]
	editor_file_dialog.popup_file_dialog()

func _on_browse_skel_file_pressed() -> void:
	editor_file_dialog.file_mode = EditorFileDialog.FILE_MODE_OPEN_FILE
	editor_file_dialog.filters = ["*.skel;骨架"]
	editor_file_dialog.popup_file_dialog()

func _on_browse_texture_file_pressed() -> void:
	editor_file_dialog.file_mode = EditorFileDialog.FILE_MODE_OPEN_FILE
	editor_file_dialog.filters = ["*.png;纹理"]
	editor_file_dialog.popup_file_dialog()

func _on_browse_imput_path_pressed() -> void:
	editor_file_dialog.file_mode = EditorFileDialog.FILE_MODE_SAVE_FILE
	editor_file_dialog.filters = ["*.tscn;场景"]
	editor_file_dialog.popup_file_dialog()

func file_selected(path: String) -> void:
	if path.get_extension() == "atlas": $VBoxContainer/HBoxContainer/AtlasFilePathInput.text = path
	elif path.get_extension() == "skel": $VBoxContainer/HBoxContainer2/SkelFilePathInput.text = path
	elif path.get_extension() == "png": $VBoxContainer/HBoxContainer4/TextureFilePathInput.text = path
	elif path.get_extension() == "tscn": $VBoxContainer/HBoxContainer3/ImputPathInput.text = path

func _on_imput_spine_pressed() -> void:
	var spine_importer = SpineImporter.new()
	if spine_importer.load(
		$VBoxContainer/HBoxContainer/AtlasFilePathInput.text,
		$VBoxContainer/HBoxContainer2/SkelFilePathInput.text,
		$VBoxContainer/HBoxContainer4/TextureFilePathInput.text
	) != OK:
		push_error("加载文件失败")
	else:
		var spine_scene = PackedScene.new()
		if spine_scene.pack(spine_importer.to_node2d("Spine")) != OK:
			push_error("保存为场景失败")
		else:
			ResourceSaver.save(spine_scene, $VBoxContainer/HBoxContainer3/ImputPathInput.text)
	spine_importer.free()
