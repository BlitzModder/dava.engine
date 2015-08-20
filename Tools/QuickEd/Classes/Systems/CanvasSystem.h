/*==================================================================================
    Copyright (c) 2008, binaryzebra
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    * Neither the name of the binaryzebra nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE binaryzebra AND CONTRIBUTORS "AS IS" AND
    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL binaryzebra BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
=====================================================================================*/


#ifndef __QUICKED_CANVAS_SYSTEM_H__
#define __QUICKED_CANVAS_SYSTEM_H__

#include "Interfaces.h"
#include "Defines.h"

#include "Base/ScopedPtr.h"

class Document;
namespace DAVA
{
    class UIControl;
}

class CanvasSystem : public SelectionInterface
{
public:
    CanvasSystem(Document *parent);
    virtual ~CanvasSystem() = default;
    void Attach(DAVA::UIControl *root);
    void Detach();
    void SelectionWasChanged(const SelectedControls &selected, const SelectedControls &deselected) override;

private:
    void SetRootControls(const SelectedNodes &controls);
    void LayoutCanvas();
    Document *document = nullptr;
    DAVA::UIControl *attachedRoot;
    DAVA::ScopedPtr<DAVA::UIControl> canvas;
    SelectedControls selectedControls;
};

#endif // __QUICKED_CANVAS_SYSTEM_H__
