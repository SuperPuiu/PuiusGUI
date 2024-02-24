void DrawCursor() {
    int CursorX, CursorY = 1;
    char *alloc = (char*)malloc(Cursor * sizeof(char) + 1);
    alloc[Cursor] = '\0';
    struct GuiProperties GUI = *GuiArray[CurrentGUI_Focused];

    char s = ' ';

    for (int i = 0; i < Cursor; i++) {
        if (GUI.Text[i] == '\n') {
            CursorX = 0;
            CursorY += 1;
        }

        CursorX++;

        if (GUI.Text[i] != '\n')
            strncat(alloc, &GUI.Text[i], 1);
        else
            strncat(alloc, &s, 1);
    }

    int TextWidth, TextHeight;

    TTF_SizeUTF8(GUI.Font, alloc, &TextWidth, &TextHeight);
    CursorX = (CursorX * TextWidth) + GUI.TextRectangle.x;
    CursorY = CursorY * GUI.TextSize + GUI.TextRectangle.y;

    printf("Text: %s\n", alloc);
    printf("CursorX: %i, CursorY: %i, TextWidth: %i, TextHeight: %i\n", CursorX, CursorY, TextWidth, TextHeight);

    DrawLine(CursorX, CursorY, CursorX, CursorY + 10, WHITE);
    free(alloc);
}
