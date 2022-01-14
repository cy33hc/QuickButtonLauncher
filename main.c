#include <taihen.h>
#include <vitasdkkern.h>
#include <psp2kern/appmgr.h>

static SceUID g_hooks[14];

uint32_t current_pid = 0, shell_pid = 0;

int checkButtons(int port, tai_hook_ref_t ref_hook, SceCtrlData *ctrl, int count) {
	int ret = 0;
	if (ref_hook == 0)
		ret = 1;
	else {
		ret = TAI_CONTINUE(int, ref_hook, port, ctrl, count);
		if (ctrl->buttons & SCE_CTRL_L1 && ctrl->buttons & SCE_CTRL_R1 && ctrl->buttons & SCE_CTRL_TRIANGLE) {
			//int opt[52/4];
			//memset(opt, 0, sizeof(opt));
			//opt[0] = sizeof(opt);
			//ksceAppMgrLaunchAppByPath("ux0:app/SMLA00001/eboot.bin", 0, 0, 0, opt, NULL);
			sceAppMgrLaunchAppByUri(0xFFFFF, "psgm:play?titleid=SMLA00001");
		}
	}
	return ret;
}

static tai_hook_ref_t ref_hook0;
static int keys_patched0(int port, SceCtrlData *ctrl, int count) {
	return checkButtons(port, ref_hook0, ctrl, count);
}   

static tai_hook_ref_t ref_hook1;
static int keys_patched1(int port, SceCtrlData *ctrl, int count) {
	return checkButtons(port, ref_hook1, ctrl, count);
}   

static tai_hook_ref_t ref_hook2;
static int keys_patched2(int port, SceCtrlData *ctrl, int count) {
	return checkButtons(port, ref_hook2, ctrl, count);
}  

static tai_hook_ref_t ref_hook3;
static int keys_patched3(int port, SceCtrlData *ctrl, int count) {
	return checkButtons(port, ref_hook3, ctrl, count);
}    

static tai_hook_ref_t ref_hook4;
static int keys_patched4(int port, SceCtrlData *ctrl, int count) {
	return checkButtons(port, ref_hook4, ctrl, count);
}    

static tai_hook_ref_t ref_hook5;
static int keys_patched5(int port, SceCtrlData *ctrl, int count) {
	return checkButtons(port, ref_hook5, ctrl, count);
}    

static tai_hook_ref_t ref_hook6;
static int keys_patched6(int port, SceCtrlData *ctrl, int count) {
	return checkButtons(port, ref_hook6, ctrl, count);
}    

static tai_hook_ref_t ref_hook7;
static int keys_patched7(int port, SceCtrlData *ctrl, int count) {
	return checkButtons(port, ref_hook7, ctrl, count);
}    

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args) {

	tai_module_info_t tai_info;
	
	tai_info.size = sizeof(tai_module_info_t);

	taiGetModuleInfoForKernel(KERNEL_PID, "SceCtrl", &tai_info);
	g_hooks[0] = taiHookFunctionExportForKernel(KERNEL_PID, &ref_hook0, "SceCtrl", TAI_ANY_LIBRARY, 0xEA1D3A34, keys_patched0); // sceCtrlPeekBufferPositive
	g_hooks[1] = taiHookFunctionOffsetForKernel(KERNEL_PID, &ref_hook1, tai_info.modid, 0, 0x3EF8, 1, keys_patched1); // sceCtrlPeekBufferPositive2
	g_hooks[2] = taiHookFunctionExportForKernel(KERNEL_PID, &ref_hook2, "SceCtrl", TAI_ANY_LIBRARY, 0x9B96A1AA, keys_patched2); // sceCtrlReadBufferPositive
	g_hooks[3] = taiHookFunctionOffsetForKernel(KERNEL_PID, &ref_hook3, tai_info.modid, 0, 0x4E14, 1, keys_patched3); // sceCtrlReadBufferPositiveExt2
	g_hooks[4] = taiHookFunctionOffsetForKernel(KERNEL_PID, &ref_hook4, tai_info.modid, 0, 0x4B48, 1, keys_patched4); // sceCtrlPeekBufferPositiveExt2
	g_hooks[5] = taiHookFunctionOffsetForKernel(KERNEL_PID, &ref_hook5, tai_info.modid, 0, 0x3928, 1, keys_patched5); // sceCtrlPeekBufferPositiveExt
    g_hooks[6] = taiHookFunctionOffsetForKernel(KERNEL_PID, &ref_hook6, tai_info.modid, 0, 0x449C, 1, keys_patched6); // sceCtrlReadBufferPositive2
    g_hooks[7] = taiHookFunctionOffsetForKernel(KERNEL_PID, &ref_hook7, tai_info.modid, 0, 0x3BCC, 1, keys_patched7); // sceCtrlReadBufferPositiveExt
		
	
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args) {
	// free hooks that didn't fail
	if (g_hooks[0] >= 0) taiHookReleaseForKernel(g_hooks[0], ref_hook0);
	if (g_hooks[1] >= 0) taiHookReleaseForKernel(g_hooks[1], ref_hook1);
	if (g_hooks[2] >= 0) taiHookReleaseForKernel(g_hooks[2], ref_hook2);
	if (g_hooks[3] >= 0) taiHookReleaseForKernel(g_hooks[3], ref_hook3);
	if (g_hooks[4] >= 0) taiHookReleaseForKernel(g_hooks[4], ref_hook4);
	if (g_hooks[5] >= 0) taiHookReleaseForKernel(g_hooks[5], ref_hook5);
	if (g_hooks[6] >= 0) taiHookReleaseForKernel(g_hooks[6], ref_hook6);
	if (g_hooks[7] >= 0) taiHookReleaseForKernel(g_hooks[7], ref_hook7);

	return SCE_KERNEL_STOP_SUCCESS;
}
