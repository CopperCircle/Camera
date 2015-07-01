#import <UIKit/UIKit.h>
#import <AssetsLibrary/AssetsLibrary.h>

@class cameraPickerViewController;

class BBCameraPicker : public Object {
    cameraPickerViewController *vc = nil;
private:
    void initNative();
   
    
public:
    void OpenCameraPickerAsync( String imagpath);
    void setImageSize(int width, int height);
    void setScaleType(int type);
    void setDeleteLastPhoto(bool value);
    bool IsRunning();
    String GetResult();
    bool HasCamera();
};


// ------------------------------------------------------------------------
//objc header
@interface cameraPickerViewController : UIViewController  <UINavigationControllerDelegate, UIImagePickerControllerDelegate>

@property (nonatomic) bool running;
@property (nonatomic,strong) NSString* imagePath;
@property (nonatomic) bool saveImage;
@property (nonatomic) String fileName;

- (id)init;
- (void)dealloc ;
- (void)takePhoto:(NSString *) imagePath;
- (bool) isRunning;
- (bool) hasCamera;
- (String) getImagePath;
- (void) saveImage:(bool)value;

@end

// ------------------------------------------------------------------------
//objc body
@implementation cameraPickerViewController

- (void)takePhoto:(NSString*) imagePath
{
    @synchronized(self) {
        self.imagePath = [imagePath stringByAppendingString:@".jpg"];
        self.running = true;
        
        BBMonkeyAppDelegate *appDelegate = BBIosGame::IosGame()->GetUIAppDelegate();
        //[self.view release];
        self.view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
        [[appDelegate view] addSubview:self.view];
        
        
        UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
        imagePicker.delegate = self;
        imagePicker.allowsEditing = YES;
        imagePicker.sourceType = UIImagePickerControllerSourceTypeCamera;
    
        [self presentViewController:imagePicker animated:YES completion:NULL];
    }
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)imagePicker {
    
    self.view.hidden = YES;
    
    self.running = false;

    [imagePicker dismissViewControllerAnimated:YES completion:NULL];
    
}

- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingImage:(UIImage *)image editingInfo:(NSDictionary *)editingInfo 
{    
    // Save image to Gallery
    if (self.saveImage == true){
         ALAssetsLibrary *library = [[ALAssetsLibrary alloc] init];
         [library writeImageToSavedPhotosAlbum:[image CGImage] orientation:(ALAssetOrientation)[image imageOrientation] completionBlock:^(NSURL *assetURL, NSError *error){
             UIAlertView *alert;
             if (error) {
                 alert = [[UIAlertView alloc] initWithTitle:@"Error" message:@"Unable to save image to Photo Album." delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil];
             } else {
                 alert = [[UIAlertView alloc] initWithTitle:@"Success" message:@"Image saved to Photo Album." delegate:self cancelButtonTitle:@"Ok" otherButtonTitles:nil];
             }
             [alert show];
             [alert release];
         }];
         [library release];
    }

    //Save image on App Folder
    NSData *imgData = UIImageJPEGRepresentation(image, 1.0);
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsPath = [paths objectAtIndex:0];
    NSString *filePath = [documentsPath stringByAppendingPathComponent:self.imagePath];

    [imgData writeToFile:filePath atomically:YES];

    self.imagePath = [@"monkey://internal/" stringByAppendingString: self.imagePath];

    self.view.hidden = YES;
    
    self.running = false;

    [picker dismissViewControllerAnimated:YES completion:NULL];

}

- (bool) isRunning{
    @synchronized(self){
        return _running;
    }
}

- (bool) hasCamera{
    @synchronized(self){
        if (![UIImagePickerController isSourceTypeAvailable:UIImagePickerControllerSourceTypeCamera]) {
            return false;
        }else {
            return true;
        }
    }
}

- (String) getImagePath{
    @synchronized(self){
        
        return self.imagePath;
    }
}


- (id)init
{
    self = [super init];
    if (self) {
        @synchronized(self){
            self.running = false;
            self.imagePath = @"";
            self.saveImage = true;
        }
    }
    
    return self;  
}

- (void) saveImage:(bool)value
{
    self.saveImage = value;
}

- (void)dealloc 
{
    [super dealloc];
}

@end


void BBCameraPicker::initNative(){
    if (vc == nil) {
        vc = [[cameraPickerViewController alloc] init];
    }
}
void BBCameraPicker::OpenCameraPickerAsync( String imagpath){
    initNative();
    
    [vc takePhoto:imagpath.ToNSString()];
}

void BBCameraPicker::setImageSize(int width, int height){

}

void BBCameraPicker::setScaleType(int type){

}

void BBCameraPicker::setDeleteLastPhoto(bool value){
    initNative();

    [vc saveImage:value];
}

bool BBCameraPicker::IsRunning(){

    return [vc isRunning];
}

String BBCameraPicker::GetResult(){
    initNative();
    return [vc getImagePath];
}

bool BBCameraPicker::HasCamera(){
    initNative();
    
    return [vc hasCamera];
}
