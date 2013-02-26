/*
 * Copyright (c) 2009, 2013, Oracle and/or its affiliates. All rights reserved.
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

package com.sun.prism.impl;

import com.sun.prism.Image;
import com.sun.prism.PixelFormat;
import com.sun.prism.Texture;
import java.nio.Buffer;

public abstract class BaseTexture extends BaseGraphicsResource implements Texture {

    private final PixelFormat format;
    private final int physicalWidth;
    private final int physicalHeight;
    private final int contentX;
    private final int contentY;
    private final int contentWidth;
    private final int contentHeight;
    // We do not provide a default wrapMode because it is so dependent on
    // how the texture will be used.
    private final WrapMode wrapMode;
    private boolean linearFiltering = true;
    private int lastImageSerial;

    protected BaseTexture(BaseTexture sharedTex, WrapMode newMode) {
        super(sharedTex);
        this.format = sharedTex.format;
        this.wrapMode = newMode;
        this.physicalWidth = sharedTex.physicalWidth;
        this.physicalHeight = sharedTex.physicalHeight;
        this.contentX = sharedTex.contentX;
        this.contentY = sharedTex.contentY;
        this.contentWidth = sharedTex.contentWidth;
        this.contentHeight = sharedTex.contentHeight;
    }

    protected BaseTexture(PixelFormat format, WrapMode wrapMode,
                          int physicalWidth, int physicalHeight,
                          int contentX, int contentY,
                          int contentWidth, int contentHeight,
                          Disposer.Record disposerRecord)
    {
        super(disposerRecord);
        this.format = format;
        this.wrapMode = wrapMode;
        this.physicalWidth = physicalWidth;
        this.physicalHeight = physicalHeight;
        this.contentX = contentX;
        this.contentY = contentY;
        this.contentWidth = contentWidth;
        this.contentHeight = contentHeight;
    }

    protected BaseTexture(PixelFormat format, WrapMode wrapMode,
                          int physicalWidth, int physicalHeight,
                          int contentX, int contentY,
                          int contentWidth, int contentHeight,
                          float u0, float v0, float u1, float v1,
                          Disposer.Record disposerRecord)
    {
        super(disposerRecord);
        this.format = format;
        this.wrapMode = wrapMode;
        this.physicalWidth = physicalWidth;
        this.physicalHeight = physicalHeight;
        this.contentX = contentX;
        this.contentY = contentY;
        this.contentWidth = contentWidth;
        this.contentHeight = contentHeight;
    }

    @Override
    public final PixelFormat getPixelFormat() {
        return format;
    }

    @Override
    public final int getPhysicalWidth() {
        return physicalWidth;
    }

    @Override
    public final int getPhysicalHeight() {
        return physicalHeight;
    }

    @Override
    public final int getContentX() {
        return contentX;
    }

    @Override
    public final int getContentY() {
        return contentY;
    }

    @Override
    public final int getContentWidth() {
        return contentWidth;
    }

    @Override
    public final int getContentHeight() {
        return contentHeight;
    }

    @Override
    public final WrapMode getWrapMode() {
        return wrapMode;
    }

    @Override
    public Texture getSharedTexture(WrapMode altMode) {
        if (wrapMode == altMode) {
            return this;
        }
        switch (altMode) {
            case REPEAT:
                if (wrapMode != WrapMode.CLAMP_TO_EDGE) {
                    return null;
                }
                break;
            case CLAMP_TO_EDGE:
                if (wrapMode != WrapMode.REPEAT) {
                    return null;
                }
            default:
                return null;
        }
        return createSharedTexture(altMode);
    }

    protected abstract Texture createSharedTexture(WrapMode newMode);

    @Override
    public final boolean getLinearFiltering() {
        return linearFiltering;
    }

    @Override
    public void setLinearFiltering(boolean linear) {
        this.linearFiltering = linear;
    }

    @Override
    public final int getLastImageSerial() {
        return lastImageSerial;
    }

    @Override
    public final void setLastImageSerial(int serial) {
        lastImageSerial = serial;
    }

    @Override
    public void update(Image img) {
        update(img, 0, 0);
    }

    @Override
    public void update(Image img, int dstx, int dsty) {
        update(img, dstx, dsty, img.getWidth(), img.getHeight());
    }

    @Override
    public void update(Image img, int dstx, int dsty, int w, int h) {
        update(img, dstx, dsty, w, h, false);
    }

    @Override
    public void update(Image img, int dstx, int dsty, int srcw, int srch,
                       boolean skipFlush)
    {
        Buffer pbuffer = img.getPixelBuffer();
        int pos = pbuffer.position();
        update(pbuffer, img.getPixelFormat(),
               dstx, dsty, img.getMinX(), img.getMinY(),
               srcw, srch, img.getScanlineStride(),
               skipFlush);
        pbuffer.position(pos);
    }

    protected void checkUpdateParams(Buffer buf, PixelFormat fmt,
                                     int dstx, int dsty,
                                     int srcx, int srcy,
                                     int srcw, int srch,
                                     int srcscan)
    {
        if (format == PixelFormat.MULTI_YCbCr_420) {
            throw new IllegalArgumentException("MULTI_YCbCr_420 requires multitexturing");
        }
        if (buf == null) {
            throw new IllegalArgumentException("Pixel buffer must be non-null");
        }
        if (fmt != format) {
            throw new IllegalArgumentException(
                "Image format (" + fmt + ") " +
                "must match texture format (" + format + ")");
        }
        if (dstx < 0 || dsty < 0) {
            throw new IllegalArgumentException(
                "dstx (" + dstx + ") and dsty (" + dsty + ") must be >= 0");
        }
        if (srcx < 0 || srcy < 0) {
            throw new IllegalArgumentException(
                "srcx (" + srcx + ") and srcy (" + srcy + ") must be >= 0");
        }
        if (srcw <= 0 || srch <= 0) {
            throw new IllegalArgumentException(
                "srcw (" + srcw + ") and srch (" + srch + ") must be > 0");
        }
        int bytesPerPixel = fmt.getBytesPerPixelUnit();
        if (srcscan % bytesPerPixel != 0) {
            throw new IllegalArgumentException(
                "srcscan (" + srcscan + ") " +
                "must be a multiple of the pixel stride (" + bytesPerPixel + ")");
        }
        if (srcw > srcscan / bytesPerPixel) {
            throw new IllegalArgumentException(
                "srcw (" + srcw + ") " +
                "must be <= srcscan/bytesPerPixel ("
                + (srcscan/bytesPerPixel) + ")");
        }
        if (dstx+srcw > contentWidth || dsty+srch > contentHeight) {
            throw new IllegalArgumentException(
                "Destination region " +
                "(x=" + dstx + ", y=" + dsty +
                ", w=" + srcw + ", h=" + srch + ") " +
                "must fit within texture content bounds " +
                "(contentWidth=" + contentWidth +
                ", contentHeight=" + contentHeight + ")");
        }
        int bytesNeeded =
            (srcx * bytesPerPixel) + (srcy * srcscan) +
            ((srch-1) * srcscan) + (srcw * bytesPerPixel);
        int elemsNeeded = bytesNeeded / format.getDataType().getSizeInBytes();
        if (elemsNeeded > buf.remaining()) {
            throw new IllegalArgumentException(
                "Upload requires " + elemsNeeded + " elements, but only " +
                buf.remaining() + " elements remain in the buffer");
        }
    }

    @Override
    public String toString() {
        return super.toString() + " [format="+format+
            " physicalWidth="+physicalWidth+
            " physicalHeight="+physicalHeight+
            " contentX="+contentX+
            " contentY="+contentY+
            " contentWidth="+contentWidth+
            " contentHeight="+contentHeight+
            " wrapMode="+wrapMode+
            " linearFiltering="+linearFiltering+
            "]";
    }
}