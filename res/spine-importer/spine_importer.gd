@tool
extends Control

var editor_file_dialog=EditorFileDialog.new()

func _enter_tree() -> void:
	#文件选择弹窗
	editor_file_dialog.access=EditorFileDialog.ACCESS_FILESYSTEM
	editor_file_dialog.transient=true
	editor_file_dialog.transient_to_focused=true
	editor_file_dialog.file_selected.connect(file_selected)
	editor_file_dialog.dir_selected.connect(dir_selected)
	get_tree().root.add_child(editor_file_dialog)

func _exit_tree() -> void:
	editor_file_dialog.queue_free()

func _on_browse_atlas_file_pressed() -> void:
	editor_file_dialog.file_mode=EditorFileDialog.FILE_MODE_OPEN_FILE
	editor_file_dialog.filters=["*.atlas;图集"]
	editor_file_dialog.popup_file_dialog()

func _on_browse_skel_file_pressed() -> void:
	editor_file_dialog.file_mode=EditorFileDialog.FILE_MODE_OPEN_FILE
	editor_file_dialog.filters=["*.skel;骨架"]
	editor_file_dialog.popup_file_dialog()

func file_selected(path:String)->void:
	if path.get_extension()=="atlas":$VBoxContainer/HBoxContainer/AtlasFilePathInput.text=path
	elif path.get_extension()=="skel":$VBoxContainer/HBoxContainer2/SkelFilePathInput.text=path

func _on_browse_imput_path_pressed() -> void:
	editor_file_dialog.file_mode=EditorFileDialog.FILE_MODE_OPEN_DIR
	editor_file_dialog.popup_file_dialog()

func dir_selected(path: String)->void:
	$VBoxContainer/HBoxContainer3/ImputPath.text=path

func _on_imput_spine_pressed() -> void:
	pass
