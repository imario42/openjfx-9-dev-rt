/*
 * Copyright (c) 2011, Oracle and/or its affiliates. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 only, as
 * published by the Free Software Foundation.  Oracle designates this
 * particular file as subject to the "Classpath" exception as provided
 * by Oracle in the LICENSE file that accompanied this code.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * You should have received a copy of the GNU General Public License version
 * 2 along with this work; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Please contact Oracle, 500 Oracle Parkway, Redwood Shores, CA 94065 USA
 * or visit www.oracle.com if you need additional information or have any
 * questions.
 */

package com.sun.javafx.scene.control.behavior;

import javafx.animation.KeyFrame;
import javafx.animation.Timeline;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.event.ActionEvent;
import javafx.event.EventHandler;
import javafx.geometry.HorizontalDirection;
import javafx.scene.control.IndexRange;
import javafx.scene.control.TextField;
import javafx.scene.input.KeyEvent;
import javafx.scene.input.MouseEvent;
import javafx.util.Duration;
import java.util.List;

import com.sun.javafx.scene.control.skin.TextFieldSkin;
import com.sun.javafx.scene.text.HitInfo;

/**
 * Text field behavior.
 */
public class TextFieldBehavior extends TextInputControlBehavior<TextField> {
    public static final int SCROLL_RATE = 15;
    private TextFieldSkin skin;
    private HorizontalDirection scrollDirection = null;
    private Timeline scrollSelectionTimeline = new Timeline();
    private EventHandler<ActionEvent> scrollSelectionHandler = new EventHandler<ActionEvent>() {
        @Override
        public void handle(ActionEvent event) {
            TextField textField = getControl();

            IndexRange selection = textField.getSelection();
            int start = selection.getStart();
            int end = selection.getEnd();

            switch (scrollDirection) {
                case RIGHT: {
                    if (end < textField.getLength()) {
                        end++;
                        textField.selectRange(start, end);
                    }

                    break;
                }

                case LEFT: {
                    if (start > 0) {
                        start--;
                        textField.selectRange(start, end);
                    }

                    break;
                }

                default: {
                    throw new RuntimeException();
                }
            }
        }
    };

    public TextFieldBehavior(TextField textField) {
        super(textField);
        // Initialize scroll timeline
        scrollSelectionTimeline.setCycleCount(Timeline.INDEFINITE);
        List<KeyFrame> scrollTimelineKeyFrames = scrollSelectionTimeline.getKeyFrames();
        scrollTimelineKeyFrames.add(new KeyFrame(Duration.millis(SCROLL_RATE), scrollSelectionHandler));
        handleFocusChange();

        // Register for change events
        textField.focusedProperty().addListener(new ChangeListener<Boolean>() {
            @Override
            public void changed(ObservableValue<? extends Boolean> observable, Boolean oldValue, Boolean newValue) {
                handleFocusChange();
            }
        });
    }

    private void handleFocusChange() {
        TextField textField = getControl();

        if (textField.isFocused()) {
            if (!focusGainedByMouseClick) {
                textField.selectRange(textField.getLength(), 0);
                setCaretAnimating(true);
            }
        } else {
            textField.selectRange(0, 0);
            focusGainedByMouseClick = false;
        }
    }

    // An unholy back-reference!
    public void setTextFieldSkin(TextFieldSkin skin) {
        this.skin = skin;
    }

    @Override protected void fire(KeyEvent event) {
        TextField textField = getControl();

        if (textField.getOnAction() != null) {
            textField.fireEvent(new ActionEvent(textField, null));
        } else if (textField.getParent() != null) {
            textField.getParent().fireEvent(event);
        }
    }

    @Override protected void deleteChar(boolean previous) {
        skin.deleteChar(previous);
    }

    @Override protected void replaceText(int start, int end, String txt) {
        skin.replaceText(start, end, txt);
    }

    @Override protected void setCaretAnimating(boolean play) {
        if (skin != null) {
            skin.setCaretAnimating(play);
        }
    }

    /**
     * Function which beeps. This requires a hook into the toolkit, and should
     * also be guarded by something that indicates whether we should beep
     * (as it is pretty annoying and many native controls don't do it).
     */
    private void beep() {
        // TODO
    }

    /**
     * If the focus is gained via response to a mouse click, then we don't
     * want to select all the text even if selectOnFocus is true.
     */
    private boolean focusGainedByMouseClick = false;
    private boolean shiftDown = false;
    private boolean deferClick = false;

    @Override public void mousePressed(MouseEvent e) {
        TextField textField = getControl();
        super.mousePressed(e);
        // We never respond to events if disabled
        if (!textField.isDisabled()) {
            // If the text field doesn't have focus, then we'll attempt to set
            // the focus and we'll indicate that we gained focus by a mouse
            // click, which will then NOT honor the selectOnFocus variable
            // of the textInputControl
            if (!textField.isFocused()) {
                focusGainedByMouseClick = true;
                textField.requestFocus();
            }

            // stop the caret animation
            setCaretAnimating(false);
            // only if there is no selection should we see the caret
//            setCaretOpacity(if (textInputControl.dot == textInputControl.mark) then 1.0 else 0.0);

            // if the primary button was pressed
            if (e.isPrimaryButtonDown() && !(e.isMiddleButtonDown() || e.isSecondaryButtonDown())) {
                HitInfo hit = skin.getIndex(e);
                int i = hit.getInsertionIndex();
                final int anchor = textField.getAnchor();
                final int caretPosition = textField.getCaretPosition();
                if (e.getClickCount() < 2 &&
                        anchor != caretPosition &&
                        ((i > anchor && i < caretPosition) || (i < anchor && i > caretPosition))) {
                    // if there is a selection, then we will NOT handle the
                    // press now, but will defer until the release. If you
                    // select some text and then press down, we change the
                    // caret and wait to allow you to drag the text (TODO).
                    // When the drag concludes, then we handle the click

                    deferClick = true;
                    // TODO start a timer such that after some millis we
                    // switch into text dragging mode, change the cursor
                    // to indicate the text can be dragged, etc.
                } else if (!(e.isControlDown() || e.isAltDown() || e.isShiftDown() || e.isMetaDown())) {
                    switch (e.getClickCount()) {
                        case 1: mouseSingleClick(hit); break;
                        case 2: mouseDoubleClick(hit); break;
                        case 3: mouseTripleClick(hit); break;
                    }
                } else if (e.isShiftDown() && !(e.isControlDown() || e.isAltDown() || e.isMetaDown()) && e.getClickCount() == 1) {
                    // didn't click inside the selection, so select
                    shiftDown = true;
                    // if we are on mac os, then we will accumulate the
                    // selection instead of just moving the dot. This happens
                    // by figuring out past which (dot/mark) are extending the
                    // selection, and set the mark to be the other side and
                    // the dot to be the new position.
                    // everywhere else we just move the dot.
                    if(macOS) {
                        textField.extendSelection(i);
                    } else {
                        skin.positionCaret(hit, true);
                    }
                }
                skin.setForwardBias(hit.isLeading());
//                if (textInputControl.editable)
//                    displaySoftwareKeyboard(true);
            }
        }
    }

    @Override public void mouseDragged(MouseEvent e) {
        final TextField textField = getControl();
        // we never respond to events if disabled, but we do notify any onXXX
        // event listeners on the control
        if (!textField.isDisabled() && !deferClick) {
            if (e.isPrimaryButtonDown() && !(e.isMiddleButtonDown() || e.isSecondaryButtonDown())) {
                if (!(e.isControlDown() || e.isAltDown() || e.isShiftDown() || e.isMetaDown())) {
                    skin.positionCaret(skin.getIndex(e), true);
                }
            }
        }
    }

    @Override public void mouseReleased(MouseEvent e) {
        final TextField textField = getControl();
        super.mouseReleased(e);
        // we never respond to events if disabled, but we do notify any onXXX
        // event listeners on the control
        if (!textField.isDisabled()) {
            setCaretAnimating(false);
            if (deferClick) {
                deferClick = false;
                skin.positionCaret(skin.getIndex(e), shiftDown);
                shiftDown = false;
            }
            setCaretAnimating(true);
        }
    }

//    var hadFocus = false;
//    var focused = bind (skin.control as TextInputControl).focused on replace old {
//        if (focused) {
//            hadFocus = true;
//            focusChanged(true);
//        } else {
//            if (hadFocus) {
//                focusChanged(false);
//            }
//            hadFocus = false;
//        }
//    }
//
//    protected function focusChanged(f:Boolean):Void {
//        def textInputControl = skin.control as TextInputControl;
//        if (f and textInputControl.selectOnFocus and not focusGainedByMouseClick) {
//            textInputControl.selectAll();
//        } else if (not f) {
//            textInputControl.commit();
//            focusGainedByMouseClick = false;
//            displaySoftwareKeyboard(false);
//        }
//    }
//
    protected void mouseSingleClick(HitInfo hit) {
        skin.positionCaret(hit, false);
    }

    protected void mouseDoubleClick(HitInfo hit) {
        final TextField textField = getControl();
        textField.previousWord();
        textField.selectNextWord();
    }

    protected void mouseTripleClick(HitInfo hit) {
        getControl().selectAll();
    }
}
