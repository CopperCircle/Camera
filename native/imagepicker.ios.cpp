#import <UIKit/UIKit.h>
#import <AssetsLibrary/AssetsLibrary.h>

@class imagePickerViewController;

class BBImagePicker : public Object {
    imagePickerViewController *vc = nil;
private:
    void initNative();
   
    
public:
    void OpenImagePickerAsync( String imagpath);
    void setImageSize(int width, int height);
    void setScaleType(int type);
    bool IsRunning();
    String GetResult();
};


// ------------------------------------------------------------------------
//objc header
@interface imagePickerViewController : UIViewController  <UINavigationControllerDelegate, UIImagePickerControllerDelegate>

@property (nonatomic) bool running;
@property (nonatomic,strong) NSString* imagePath;

- (id)init;
- (void)dealloc;
- (void)selectPhoto:(NSString *) imagePath;
- (bool) isRunning;
- (String) getImagePath;
@end

// ------------------------------------------------------------------------
//objc body
@implementation imagePickerViewController

- (void)selectPhoto:(NSString*) imagePath
{
    @synchronized(self) {
        self.running = true;
        self.imagePath = [imagePath stringByAppendingString:@".jpg"];
        
        BBMonkeyAppDelegate *appDelegate = BBIosGame::IosGame()->GetUIAppDelegate();
        //[self.view release];
        self.view = [[UIView alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
        [[appDelegate view] addSubview:self.view];
        
        
        UIImagePickerController *imagePicker = [[UIImagePickerController alloc] init];
        imagePicker.delegate = self;
        imagePicker.allowsEditing = YES;
        imagePicker.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    
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
        return self.running;
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
        }
    }
    
    return self;  
}

- (void)dealloc 
{
    [super dealloc];
}

@end


void BBImagePicker::initNative(){
    if (vc == nil) {
        vc = [[imagePickerViewController alloc] init];
    }
}
void BBImagePicker::OpenImagePickerAsync( String imagpath){
    initNative();
    
    [vc selectPhoto:imagpath.ToNSString()];
}

void BBImagePicker::setImageSize(int width, int height){

}

void BBImagePicker::setScaleType(int type){

}

bool BBImagePicker::IsRunning(){
    initNative();

    return [vc isRunning];
}

String BBImagePicker::GetResult(){
    initNative();

    return [vc getImagePath];
}