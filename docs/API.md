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

### Text

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
### Type `Type` `READ ONLY`
An `enum` used by the library to adjust different properties, and to generate different behaviour. After constructing the element, you should not change it to something else.
The `Type` property can have the following values:

`TEXTLABEL` - Used for constructing text labels.

`TEXTBOX` - Used for constructing text labels. The library looks for this property whenever the left mouse button is down and the cursor is hovering over an element.

`TEXTBUTTON` - Used for constructing buttons. The library will automatically fire `MouseDown` and `MouseUp` callbacks.

`IMAGELABEL` - Used for constructing image labels. The library will try to render any specified image attached to the struct.

`IMAGEBUTTON` - Used for constructing image buttons. Behaves like `TextButton` and `ImageLabel`.

`UILIST` - Used for constructing lists. When parented to anything than -1, it will position everything under the same parent in a list.
### TextXAlignment `TextXAlignment`
An `enum` used by the library to automatically position the X coordinate of the text. Setting it to anything but the accepted enums will result in undefined behaviour.
### TextYAlignment `TextYAlignment`

## Callback Properties
### MouseDown `Function pointer`

### MouseEnter `Function pointer`

### MouseLeave `Function pointer`

### FocusLost `Function pointer`

# Functions
## General Functions

## Constructors

# Header extern values
## GuiArray `struct GuiProperties`

## LastGUI_Item `int`

## IsFocused `bool`

## CurrentGUI_Focused `int`

## Inputs `int*`

## Running `int` `READ ONLY`
Used by the `ProcessInputs()` function. The value is set to 0 whenever the `SDL_QUIT` event is encountered. Should be used for creating the game's loop.
