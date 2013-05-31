/* 
 * Copyright (c) 2011, 2013, Oracle and/or its affiliates. All rights reserved.
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

package javafx.scene.effect;

/**
Builder class for javafx.scene.effect.DisplacementMap
@see javafx.scene.effect.DisplacementMap
@deprecated This class is deprecated and will be removed in the next version
* @since JavaFX 2.0
*/
@javax.annotation.Generated("Generated by javafx.builder.processor.BuilderProcessor")
@Deprecated
public class DisplacementMapBuilder<B extends javafx.scene.effect.DisplacementMapBuilder<B>> implements javafx.util.Builder<javafx.scene.effect.DisplacementMap> {
    protected DisplacementMapBuilder() {
    }
    
    /** Creates a new instance of DisplacementMapBuilder. */
    @SuppressWarnings({"deprecation", "rawtypes", "unchecked"})
    public static javafx.scene.effect.DisplacementMapBuilder<?> create() {
        return new javafx.scene.effect.DisplacementMapBuilder();
    }
    
    private int __set;
    public void applyTo(javafx.scene.effect.DisplacementMap x) {
        int set = __set;
        if ((set & (1 << 0)) != 0) x.setInput(this.input);
        if ((set & (1 << 1)) != 0) x.setMapData(this.mapData);
        if ((set & (1 << 2)) != 0) x.setOffsetX(this.offsetX);
        if ((set & (1 << 3)) != 0) x.setOffsetY(this.offsetY);
        if ((set & (1 << 4)) != 0) x.setScaleX(this.scaleX);
        if ((set & (1 << 5)) != 0) x.setScaleY(this.scaleY);
        if ((set & (1 << 6)) != 0) x.setWrap(this.wrap);
    }
    
    private javafx.scene.effect.Effect input;
    /**
    Set the value of the {@link javafx.scene.effect.DisplacementMap#getInput() input} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B input(javafx.scene.effect.Effect x) {
        this.input = x;
        __set |= 1 << 0;
        return (B) this;
    }
    
    private javafx.scene.effect.FloatMap mapData;
    /**
    Set the value of the {@link javafx.scene.effect.DisplacementMap#getMapData() mapData} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B mapData(javafx.scene.effect.FloatMap x) {
        this.mapData = x;
        __set |= 1 << 1;
        return (B) this;
    }
    
    private double offsetX;
    /**
    Set the value of the {@link javafx.scene.effect.DisplacementMap#getOffsetX() offsetX} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B offsetX(double x) {
        this.offsetX = x;
        __set |= 1 << 2;
        return (B) this;
    }
    
    private double offsetY;
    /**
    Set the value of the {@link javafx.scene.effect.DisplacementMap#getOffsetY() offsetY} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B offsetY(double x) {
        this.offsetY = x;
        __set |= 1 << 3;
        return (B) this;
    }
    
    private double scaleX;
    /**
    Set the value of the {@link javafx.scene.effect.DisplacementMap#getScaleX() scaleX} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B scaleX(double x) {
        this.scaleX = x;
        __set |= 1 << 4;
        return (B) this;
    }
    
    private double scaleY;
    /**
    Set the value of the {@link javafx.scene.effect.DisplacementMap#getScaleY() scaleY} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B scaleY(double x) {
        this.scaleY = x;
        __set |= 1 << 5;
        return (B) this;
    }
    
    private boolean wrap;
    /**
    Set the value of the {@link javafx.scene.effect.DisplacementMap#isWrap() wrap} property for the instance constructed by this builder.
    */
    @SuppressWarnings("unchecked")
    public B wrap(boolean x) {
        this.wrap = x;
        __set |= 1 << 6;
        return (B) this;
    }
    
    /**
    Make an instance of {@link javafx.scene.effect.DisplacementMap} based on the properties set on this builder.
    */
    public javafx.scene.effect.DisplacementMap build() {
        javafx.scene.effect.DisplacementMap x = new javafx.scene.effect.DisplacementMap();
        applyTo(x);
        return x;
    }
}
