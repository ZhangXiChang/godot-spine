@tool
extends EditorPlugin

var window=Window.new()

func _enter_tree() -> void:
	#Spine导入器窗口
	window.title="Spine导入器"
	window.size=Vector2i(400,300)
	window.always_on_top=true
	window.close_requested.connect(window.hide)
	window.add_child(preload("res://addons/godot-spine/res/spine-importer/spine_importer.tscn").instantiate())
	get_tree().root.add_child(window)
	window.move_to_center()
	window.hide()
	add_tool_menu_item("Spine导入器",window.show)
	
	var spine_importer=SpineImporter.new()
	if spine_importer.load(
		"C:/Users/yxrbe/Project/Godot/demo/nightingale/char_179_cgbird_sightseer_1.atlas",
		"C:/Users/yxrbe/Project/Godot/demo/nightingale/char_179_cgbird_sightseer_1.skel"
	)!=OK:
		push_error("加载文件失败")
	var scene=PackedScene.new()
	scene.pack(spine_importer.to_node2d("Spine"))
	ResourceSaver.save(scene,"res://test.tscn")
	spine_importer.free()

func _exit_tree() -> void:
	remove_tool_menu_item("Spine导入器")
