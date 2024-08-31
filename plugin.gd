@tool
extends EditorPlugin

var window = Window.new()

func _enter_tree() -> void:
	#Spine导入器窗口
	window.title = "Spine导入器"
	window.size = Vector2i(300, 210)
	window.always_on_top = true
	window.close_requested.connect(window.hide)
	window.add_child(preload("res://addons/godot-spine/res/spine-importer/spine_importer.tscn").instantiate())
	get_tree().root.add_child(window)
	window.move_to_center()
	window.hide()
	add_tool_menu_item("Spine导入器", window.show)

func _exit_tree() -> void:
	remove_tool_menu_item("Spine导入器")
