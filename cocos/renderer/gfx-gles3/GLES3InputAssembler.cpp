/****************************************************************************
 Copyright (c) 2019-2021 Xiamen Yaji Software Co., Ltd.

 http://www.cocos.com

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated engine source code (the "Software"), a limited,
 worldwide, royalty-free, non-assignable, revocable and non-exclusive license
 to use Cocos Creator solely to develop games on your target platforms. You shall
 not use Cocos Creator software for developing other software or tools that's
 used for developing games. You are not granted to publish, distribute,
 sublicense, and/or sell copies of Cocos Creator.

 The software or tools in this License Agreement are licensed, not sold.
 Xiamen Yaji Software Co., Ltd. reserves all rights not expressly granted to you.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
****************************************************************************/

#include "GLES3Std.h"

#include "GLES3Buffer.h"
#include "GLES3Commands.h"
#include "GLES3Device.h"
#include "GLES3InputAssembler.h"

namespace cc {
namespace gfx {

GLES3InputAssembler::GLES3InputAssembler() {
    _typedID = generateObjectID<decltype(this)>();
}

GLES3InputAssembler::~GLES3InputAssembler() {
    destroy();
}

void GLES3InputAssembler::doInit(const InputAssemblerInfo &info) {
    _gpuInputAssembler             = CC_NEW(GLES3GPUInputAssembler);
    _gpuInputAssembler->attributes = _attributes;
    _gpuInputAssembler->gpuVertexBuffers.resize(_vertexBuffers.size());
    for (size_t i = 0; i < _gpuInputAssembler->gpuVertexBuffers.size(); ++i) {
        auto *vb                                = static_cast<GLES3Buffer *>(_vertexBuffers[i]);
        _gpuInputAssembler->gpuVertexBuffers[i] = vb->gpuBuffer();
    }
    if (info.indexBuffer) {
        _gpuInputAssembler->gpuIndexBuffer = static_cast<GLES3Buffer *>(info.indexBuffer)->gpuBuffer();
    }

    if (info.indirectBuffer) {
        _gpuInputAssembler->gpuIndirectBuffer = static_cast<GLES3Buffer *>(info.indirectBuffer)->gpuBuffer();
    }

    cmdFuncGLES3CreateInputAssembler(GLES3Device::getInstance(), _gpuInputAssembler);
}

void GLES3InputAssembler::doDestroy() {
    if (_gpuInputAssembler) {
        cmdFuncGLES3DestroyInputAssembler(GLES3Device::getInstance(), _gpuInputAssembler);
        CC_DELETE(_gpuInputAssembler);
        _gpuInputAssembler = nullptr;
    }
}

void GLES3InputAssembler::ExtractCmdDraw(GLES3CmdDraw *cmd) {
    cmd->drawInfo.vertexCount   = _vertexCount;
    cmd->drawInfo.firstVertex   = _firstVertex;
    cmd->drawInfo.indexCount    = _indexCount;
    cmd->drawInfo.firstIndex    = _firstIndex;
    cmd->drawInfo.vertexOffset  = _vertexOffset;
    cmd->drawInfo.instanceCount = _instanceCount;
    cmd->drawInfo.firstInstance = _firstInstance;
}

} // namespace gfx
} // namespace cc
