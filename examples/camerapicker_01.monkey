#Rem
	Title: CameraPicker example 1 - Open camera And load it in monkey
	Description: Open camera And load it into monkey.
	
	Author: Dominik Kollon
	Contact:      d.kollon@blackbird-design.de
	
	Repository: https://github.com/Ironstorm/bbd
	
	iOS version added by CopperCircle
	Repository: https://github.com/CopperCircle/Camera.git
	
	License:      MIT
#End

Import camera.camerapicker
Import mojo

Function Main()
	New MyApp
End

Class MyApp Extends App Implements IOnCameraPickComplete, IOnLoadImageComplete
	
	Field camerapicker:CameraPicker
	Field myimage:Image
	Field imagename:String = "test"
	
	Method OnCreate()
		camerapicker = New CameraPicker
		camerapicker.SetScaleType(2)															' Scale the image to fit in width and height
		camerapicker.SetImageSize(DeviceWidth(), DeviceHeight())								' Scale image to the devices resolution
		camerapicker.DeleteGalleryImage(True)													' Do not add the image to the user gallery
		SetUpdateRate(60)
	End
	
	Method OnUpdate()
		UpdateAsyncEvents
		
		If TouchHit() Then
			If camerapicker.HasCamera() Then camerapicker.OpenCameraPickerAsync("test", Self)	' Check if device has camera, if so open the camera
		EndIf
	End
	
	Method OnRender()
		Cls()
	
		If myimage DrawImage(myimage, 0, 0)
		
		DrawText("Tap on the screen to open Camera", 10, 10)
	End
	
	Method OnCameraPickComplete:Void(result:String)
		If result <> "" Then
			LoadImageAsync(result, 1, Image.DefaultFlags, Self)
		EndIf
	End
	
	Method OnLoadImageComplete:Void(image:Image, path:String, source:IAsyncEventSource)
		If path.Contains(imagename) Then
			myimage = image
		EndIf
	End
	
End