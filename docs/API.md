`READ ONLY` properties should be read from only. Changing them may result in undefined behaviour.
# GUI Properties
## General Properties
### PositionX and PositionY `int`
Two int values used to specify the positions for the GUI element. Some elements ignore this property. `ConstructGUI` takes two parameters for creating new elements.

### SizeX and SizeY `int`
Two int values used to specify the positions for the GUI element. Some elements ignore this property. The default values for SizeX is 75 and for SizeY 25.

### Parent `int`
An int value used to specify under which element is the GUI. Setting the value to anything but -1 will result in the element to change `PositionX` and `PositionY` related to the parent GUI. Setting the value to anything higher than `LastGUI_Item` 
will result in undefined behaviour.
### Zindex `int`
An int value used to specify the render priority. Setting the zindex to anythung under 1 will result in the GUI not being rendered. The maximum zindex supported is 40.
### BorderSize `int`
An int value used to specify what is the size of the GUI element border. Some elements ignore this property.
### OutlineSize `int`
An int value used to specify what is the size of the text's outline. Any element other than `TextBox` and `TextLabel` will ignore this property.
### Visible `bool`
A bool value used to specify if the GUI element will be rendered or not. 
### Pressed `bool` `READ ONLY`
A bool value which is automatically set to true if the button is being pressed. The bool is set to false if otherwise.
### Active `bool`
A bool value used by the library to specify if the GUI element will fire callbacks. *To be implemented in 0.0.5*
### Hovered `bool` `READ ONLY`
A bool value used by the library to specify if the GUI is currently being hovered or not.  
## Text Properties

### TextEditable `bool`
A bool value used to specify if the TextLabel or TextBox has editable text or not. Setting this to false for a GUI element which the user is editing will stop the text editing process.
### TextFits `bool` `READ ONLY`
A bool value used by the library to specify if the text fits or not in the boundaries of the GUI element. Should not be changed manually.
### TextScaled `bool`
A bool value used to specify if the library will handle automatically the text size or not. Setting this to true will make the library ignore TextSize property. *To be implemented in 0.0.5*
### TextSize `int`
An int value used to specify the size of the text. The value will be ignored if TextSize is enabled.
### TextureText `READ ONLY`
A `SDL_Texture` structure used by the library to render text. **Do not manually change.**
### TextRectangle `READ ONLY`
A `SDL_Rect` structure used by the library for different operations and text rendering. **Do not manually change.**
### Font `TTF_Font`
A `TTF_Font` value used by SDL_ttf for storing fonts. The font is automatically assigned by the constructor when built. Alternatively, it can be changed manually if needed.

### Text `char *`
A `char*` (string) used by `TextLabel` and `TextBox` for rendering text. It should be noted that `TextBox` has text `malloc()`ed when constructed and `TextLabel` isn't.

## Color3 Properties
### BackgroundColor `Color3`
The `Color3` struct used to specify the color of the element's background. Some elements will ignore the property.
### TextColor `Color3`
The `Color3` struct used to specify the color of the element's text. Some elements will ignore the property.
### BorderlineColor `Color3`
The `Color3` struct used to specify the color of the element's border. Some elements will ignore the property.
## Image Property `SDL_Texture`
The image property is used by `ImageButton` and `ImageLabel` to render text. The property has to be manually assigned once the struct has been created. Not assigning the property and rendering the element will result in nothing being displayed.
## Enum Properties
### Type `GUI_TYPE` `READ ONLY`
An `enum` used by the library to adjust different properties, and to generate different behaviour. After constructing the element, you should not change it to something else.
The `Type` property can have the following values:

`TEXTLABEL` - Used for constructing text labels.

`TEXTBOX` - Used for constructing text labels. The library looks for this property whenever the left mouse button is down and the cursor is hovering over an element.

`TEXTBUTTON` - Used for constructing buttons. The library will automatically fire `MouseDown` and `MouseUp` callbacks.

`IMAGELABEL` - Used for constructing image labels. The library will try to render any specified image attached to the struct.

`IMAGEBUTTON` - Used for constructing image buttons. Behaves like `TextButton` and `ImageLabel`.

`UILIST` - Used for constructing lists. When parented to anything than -1, it will position everything under the same parent in a list.
### TextXAlignment `TextXAlignment`
An `enum` used by the library to automatically position the X coordinate of the text. Setting it to anything but the accepted enums will result in undefined behaviour. `TextWrapped` may modify it.
The accepted enums are the following:
`X_CENTER` - Used to center the text on the X coordinate.

`LEFT` - Used to position to the left on the X coordinate.

`RIGHT` - Used to position to the right on the X coordinate.
### TextYAlignment `TextYAlignment`
An `enum` used by the library to automatically position the Y coordinate of the text. Setting it to anything but the accepted enums will result in undefined behaviour. `TextWrapped` may modify it.
The accepted enums are the following:
`Y_CENTER` - Used to center the text on the Y coordinate.

`TOP` - Used to position to the top on the Y coordinate.

`BOTTOM` - Used to position to the bottom on the Y coordinate.
## Callback Properties
### MouseDown `Function pointer`
A function called by the library whenever the user clicks on the GUI element, one of the arguments being the index of the GUI.
### MouseEnter `Function pointer`
A function called by the library whenever the cursor hovers over the element, one of the arguments being the index of the GUI.
### MouseLeave `Function pointer`
A function called by the library whenever the cursor leaves the element, one of the arguments being the index of the GUI.
### FocusLost `Function pointer`
A function called by the library whenever the user just finished editing the textbox, one of the arguments being the index of the GUI.
# Functions
## General Functions
### InitLayer(SDL_Renderer *renderer, SDL_Window *window) `int Function`
A core function used to initialize the library. The init function loads `arial.ttf`, ensure that `globalRenderer`,`globalWindow` and default `Color3` structs are initialized.

### UninitLayer() `void Function`
A core function used to automatically free allocated **textboxes**'s text and other elements' `TextureText` variables.

### RenderGUI() `void Function`
A core function used to render GUI buttons. Should be called after `SDL_RenderClear()`.

### UpdateGUI(int GUI_Index) `void Function`
An important function used to automatically update an element whose index is specified in the first argument.

### UpdateAllGUI() `void Function`
A function used to update all buttons at once. This function uses a for loop which begins from 0 to `LastGUI_Item`, calling `UpdateGUI`.

### ChangeDefaultFont(char *FontName, int FontSize) `int Function`
A function used to change the default font. `*FontName` should be a pointer to the location of the file.

## Constructors
### ConstructGUI(enum GUI_TYPE, int PositionX, int PositionY) `int Function`
A core function which constructs a new GUI element based on the given enum and positions. See `Type` values for more information.

### InitColor3(int R, int G, int B, int A) `struct Color3 Function`
Function used to construct more easily `Color3` structs. It returns a structure, and accepts 4 arguments.

# Header extern values
### GuiArray `struct GuiProperties`
A core structure used by the library to store and render GUI items. Can be used to modify a GUI element's properties using the index provided by `ConstructGUI()`.

### LastGUI_Item `int` `READ ONLY`
An int value which contains the last index of the `GuiArray`. Changing it may result in undefined behaviour.

### IsFocused `bool`
A bool value set by the library whenever the user is editing a textbox or not.

### CurrentGUI_Focused `int`
An int set by the library whenever the user is editing a textbox. Do not rely on it to check if the user is typing or not, and instead use `IsFocused`.

### Inputs `bool*`
A bool array used by `ProcessInput()` function to store pressed keys and other important inputs.

### Running `int`
Used by the `ProcessInputs()` function. The value is set to 0 whenever the `SDL_QUIT` event is encountered. Should be used for creating the game's loop.