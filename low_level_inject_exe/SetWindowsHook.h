#pragma once
BOOL SetWindowsHook2Inject(HANDLE processId);



///*
// * COPYRIGHT:        See COPYING in the top level directory
// * PROJECT:          ReactOS kernel
// * PURPOSE:          Window hooks
// * FILE:             subsystems/win32/win32k/ntuser/hook.c
// * PROGRAMER:        Casper S. Hornstrup (chorns@users.sourceforge.net)
// *                   James Tabor (james.tabor@rectos.org)
// *                   Rafal Harabien (rafalh@reactos.org)
//  * NOTE:            Most of this code was adapted from Wine,
// *                   Copyright (C) 2002 Alexandre Julliard
// */
//
//HHOOK
//APIENTRY
//NtUserSetWindowsHookEx(HINSTANCE Mod,
//    PUNICODE_STRING UnsafeModuleName,
//    DWORD ThreadId,
//    int HookId,
//    HOOKPROC HookProc,
//    BOOL Ansi)
//{
//    PWINSTATION_OBJECT WinStaObj;
//    PHOOK Hook;
//    UNICODE_STRING ModuleName;
//    NTSTATUS Status;
//    HHOOK Handle;
//    PETHREAD Thread = NULL;
//    PTHREADINFO pti, ptiHook = NULL;
//    DECLARE_RETURN(HHOOK);
//
//    TRACE("Enter NtUserSetWindowsHookEx\n");
//    UserEnterExclusive();
//
//    pti = PsGetCurrentThreadWin32Thread();
//
//    if (HookId < WH_MINHOOK || WH_MAXHOOK < HookId)
//    {
//        EngSetLastError(ERROR_INVALID_HOOK_FILTER);
//        RETURN(NULL);
//    }
//
//    if (!HookProc)
//    {
//        EngSetLastError(ERROR_INVALID_FILTER_PROC);
//        RETURN(NULL);
//    }
//
//    if (ThreadId)  /* thread-local hook */
//    {
//        if (HookId == WH_JOURNALRECORD ||
//            HookId == WH_JOURNALPLAYBACK ||
//            HookId == WH_KEYBOARD_LL ||
//            HookId == WH_MOUSE_LL ||
//            HookId == WH_SYSMSGFILTER)
//        {
//            ERR("Local hook installing Global HookId: %d\n", HookId);
//            /* these can only be global */
//            EngSetLastError(ERROR_GLOBAL_ONLY_HOOK);
//            RETURN(NULL);
//        }
//
//        if (!NT_SUCCESS(PsLookupThreadByThreadId((HANDLE)(DWORD_PTR)ThreadId, &Thread)))
//        {
//            ERR("Invalid thread id 0x%x\n", ThreadId);
//            EngSetLastError(ERROR_INVALID_PARAMETER);
//            RETURN(NULL);
//        }
//
//        ptiHook = Thread->Tcb.Win32Thread;
//
//        ObDereferenceObject(Thread);
//
//        if (ptiHook->rpdesk != pti->rpdesk) // gptiCurrent->rpdesk)
//        {
//            ERR("Local hook wrong desktop HookId: %d\n", HookId);
//            EngSetLastError(ERROR_ACCESS_DENIED);
//            RETURN(NULL);
//        }
//
//        if (Thread->ThreadsProcess != PsGetCurrentProcess())
//        {
//            if (!Mod &&
//                (HookId == WH_GETMESSAGE ||
//                    HookId == WH_CALLWNDPROC ||
//                    HookId == WH_CBT ||
//                    HookId == WH_HARDWARE ||
//                    HookId == WH_DEBUG ||
//                    HookId == WH_SHELL ||
//                    HookId == WH_FOREGROUNDIDLE ||
//                    HookId == WH_CALLWNDPROCRET))
//            {
//                ERR("Local hook needs hMod HookId: %d\n", HookId);
//                EngSetLastError(ERROR_HOOK_NEEDS_HMOD);
//                RETURN(NULL);
//            }
//
//            if ((ptiHook->TIF_flags & (TIF_CSRSSTHREAD | TIF_SYSTEMTHREAD)) &&
//                (HookId == WH_GETMESSAGE ||
//                    HookId == WH_CALLWNDPROC ||
//                    HookId == WH_CBT ||
//                    HookId == WH_HARDWARE ||
//                    HookId == WH_DEBUG ||
//                    HookId == WH_SHELL ||
//                    HookId == WH_FOREGROUNDIDLE ||
//                    HookId == WH_CALLWNDPROCRET))
//            {
//                EngSetLastError(ERROR_HOOK_TYPE_NOT_ALLOWED);
//                RETURN(NULL);
//            }
//        }
//    }
//    else  /* System-global hook */
//    {
//        ptiHook = pti; // gptiCurrent;
//        if (!Mod &&
//            (HookId == WH_GETMESSAGE ||
//                HookId == WH_CALLWNDPROC ||
//                HookId == WH_CBT ||
//                HookId == WH_SYSMSGFILTER ||
//                HookId == WH_HARDWARE ||
//                HookId == WH_DEBUG ||
//                HookId == WH_SHELL ||
//                HookId == WH_FOREGROUNDIDLE ||
//                HookId == WH_CALLWNDPROCRET))
//        {
//            ERR("Global hook needs hMod HookId: %d\n", HookId);
//            EngSetLastError(ERROR_HOOK_NEEDS_HMOD);
//            RETURN(NULL);
//        }
//    }
//
//    Status = IntValidateWindowStationHandle(PsGetCurrentProcess()->Win32WindowStation,
//        KernelMode,
//        0,
//        &WinStaObj);
//
//    if (!NT_SUCCESS(Status))
//    {
//        SetLastNtError(Status);
//        RETURN(NULL);
//    }
//    ObDereferenceObject(WinStaObj);
//
//    Hook = UserCreateObject(gHandleTable, NULL, &Handle, otHook, sizeof(HOOK));
//
//    if (!Hook)
//    {
//        RETURN(NULL);
//    }
//
//    Hook->ihmod = (INT)Mod; // Module Index from atom table, Do this for now.
//    Hook->Thread = Thread; /* Set Thread, Null is Global. */
//    Hook->HookId = HookId;
//    Hook->rpdesk = ptiHook->rpdesk;
//    Hook->phkNext = NULL; /* Dont use as a chain! Use link lists for chaining. */
//    Hook->Proc = HookProc;
//    Hook->Ansi = Ansi;
//
//    TRACE("Set Hook Desk 0x%x DeskInfo 0x%x Handle Desk 0x%x\n", pti->rpdesk, pti->pDeskInfo, Hook->head.rpdesk);
//
//    if (ThreadId)  /* Thread-local hook */
//    {
//        InsertHeadList(&ptiHook->aphkStart[HOOKID_TO_INDEX(HookId)], &Hook->Chain);
//        ptiHook->sphkCurrent = NULL;
//        Hook->ptiHooked = ptiHook;
//        ptiHook->fsHooks |= HOOKID_TO_FLAG(HookId);
//
//        if (ptiHook->pClientInfo)
//        {
//            if (ptiHook->ppi == pti->ppi) /* gptiCurrent->ppi) */
//            {
//                _SEH2_TRY
//                {
//                   ptiHook->pClientInfo->fsHooks = ptiHook->fsHooks;
//                   ptiHook->pClientInfo->phkCurrent = NULL;
//                }
//                    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
//                {
//                    ERR("Problem writing to Local ClientInfo!\n");
//                }
//                _SEH2_END;
//            }
//            else
//            {
//                KeAttachProcess(&ptiHook->ppi->peProcess->Pcb);
//                _SEH2_TRY
//                {
//                   ptiHook->pClientInfo->fsHooks = ptiHook->fsHooks;
//                   ptiHook->pClientInfo->phkCurrent = NULL;
//                }
//                    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
//                {
//                    ERR("Problem writing to Remote ClientInfo!\n");
//                }
//                _SEH2_END;
//                KeDetachProcess();
//            }
//        }
//    }
//    else
//    {
//        InsertHeadList(&ptiHook->rpdesk->pDeskInfo->aphkStart[HOOKID_TO_INDEX(HookId)], &Hook->Chain);
//        Hook->ptiHooked = NULL;
//        //gptiCurrent->pDeskInfo->fsHooks |= HOOKID_TO_FLAG(HookId);
//        ptiHook->rpdesk->pDeskInfo->fsHooks |= HOOKID_TO_FLAG(HookId);
//        ptiHook->sphkCurrent = NULL;
//        ptiHook->pClientInfo->phkCurrent = NULL;
//    }
//
//    RtlInitUnicodeString(&Hook->ModuleName, NULL);
//
//    if (Mod)
//    {
//        Status = MmCopyFromCaller(&ModuleName,
//            UnsafeModuleName,
//            sizeof(UNICODE_STRING));
//        if (!NT_SUCCESS(Status))
//        {
//            IntRemoveHook(Hook);
//            SetLastNtError(Status);
//            RETURN(NULL);
//        }
//
//        Hook->ModuleName.Buffer = ExAllocatePoolWithTag(PagedPool,
//            ModuleName.MaximumLength,
//            TAG_HOOK);
//        if (NULL == Hook->ModuleName.Buffer)
//        {
//            IntRemoveHook(Hook);
//            EngSetLastError(ERROR_NOT_ENOUGH_MEMORY);
//            RETURN(NULL);
//        }
//
//        Hook->ModuleName.MaximumLength = ModuleName.MaximumLength;
//        Status = MmCopyFromCaller(Hook->ModuleName.Buffer,
//            ModuleName.Buffer,
//            ModuleName.MaximumLength);
//        if (!NT_SUCCESS(Status))
//        {
//            ExFreePoolWithTag(Hook->ModuleName.Buffer, TAG_HOOK);
//            Hook->ModuleName.Buffer = NULL;
//            IntRemoveHook(Hook);
//            SetLastNtError(Status);
//            RETURN(NULL);
//        }
//
//        Hook->ModuleName.Length = ModuleName.Length;
//        /* Make proc relative to the module base */
//        Hook->offPfn = (ULONG_PTR)((char*)HookProc - (char*)Mod);
//    }
//    else
//        Hook->offPfn = 0;
//
//    TRACE("Installing: HookId %d Global %s\n", HookId, !ThreadId ? "TRUE" : "FALSE");
//    RETURN(Handle);
//
//CLEANUP:
//    TRACE("Leave NtUserSetWindowsHookEx, ret=%i\n", _ret_);
//    UserLeave();
//    END_CLEANUP;
//}
//
///*
// * PROJECT:         ReactOS Kernel
// * LICENSE:         GPL - See COPYING in the top level directory
// * FILE:            ntoskrnl/ps/thread.c
// * PURPOSE:         Process Manager: Thread Management
// * PROGRAMMERS:     Alex Ionescu (alex.ionescu@reactos.org)
// *                  Thomas Weidenmueller (w3seek@reactos.org)
// */
//NTSTATUS
//NTAPI
//PsLookupThreadByThreadId(IN HANDLE ThreadId,
//    OUT PETHREAD* Thread)
//{
//    PHANDLE_TABLE_ENTRY CidEntry;
//    PETHREAD FoundThread;
//    NTSTATUS Status = STATUS_INVALID_PARAMETER;
//    PAGED_CODE();
//    PSTRACE(PS_THREAD_DEBUG, "ThreadId: %p\n", ThreadId);
//    KeEnterCriticalRegion();
//
//    /* Get the CID Handle Entry */
//    CidEntry = ExMapHandleToPointer(PspCidTable, ThreadId);
//    if (CidEntry)
//    {
//        /* Get the Process */
//        FoundThread = CidEntry->Object;
//
//        /* Make sure it's really a process */
//        if (FoundThread->Tcb.Header.Type == ThreadObject)
//        {
//            /* Safe Reference and return it */
//            if (ObReferenceObjectSafe(FoundThread))
//            {
//                *Thread = FoundThread;
//                Status = STATUS_SUCCESS;
//            }
//        }
//
//        /* Unlock the Entry */
//        ExUnlockHandleTableEntry(PspCidTable, CidEntry);
//    }
//
//    /* Return to caller */
//    KeLeaveCriticalRegion();
//    return Status;
//}
