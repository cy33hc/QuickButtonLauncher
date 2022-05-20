#include <vitasdk.h>
#include <taihen.h>

#define HOOKS_NUM    12 // Hooked functions num

static uint8_t current_hook = 0;
static SceUID hooks[HOOKS_NUM];
static tai_hook_ref_t refs[HOOKS_NUM];
static char uri[64];
static unsigned int buttons = 0;

void setDefault()
{
	sceClibSnprintf(uri, 64, "psgm:play?titleid=VITASHELL");
	buttons = SCE_CTRL_L1 | SCE_CTRL_R1 | SCE_CTRL_SQUARE;
}

unsigned int str2int(char *str, int len)
{
    unsigned int temp = 0;
    for (int i = 0; i < len; i++)
	{
        temp = (temp * 10) + (str[i] - '0');
    }
    return temp;
}

void loadConfig()
{
	char buf[128];
	sceClibMemset(buf, 0, 128);

	int in = sceIoOpen("ux0:tai/qblauncher.ini", SCE_O_RDONLY, 0777);
	if (in <= 0)
	{
		in = sceIoOpen("ur0:tai/qblauncher.ini", SCE_O_RDONLY, 0777);
	}

	if (in <= 0 )
	{
		return;
	}
	else
	{
		SceUID out = sceIoOpen("ux0:tai/qblauncher.log", SCE_O_WRONLY | SCE_O_CREAT | SCE_O_TRUNC, 0777);
		if (out <= 0) return;

		sceIoRead(in, buf, 128);
		sceIoClose(in);

		// Strip out new line
		char *pp;
		char *p = sceClibStrrchr(buf, '\r');
		if (p != NULL) *p=0;
		p = sceClibStrrchr(buf, '\n');
		if (p != NULL) *p=0;

		p = sceClibStrrchr(buf, '=');
		if (p == NULL)
		{
			sceIoClose(out);
			return;
		}
		
		sceClibSnprintf(uri, 64, "psgm:play?titleid=%s", p+1);
		sceIoWrite(out, uri, sceClibStrnlen(uri, 64));
		*p=0;
		pp = p;
		buttons = 0;
		while ((p = sceClibStrrchr(buf, ',')) != NULL)
		{
			if (pp - p <= 0)
			{
				setDefault();
				sceIoClose(out);
				return;
			}
			buttons = buttons | str2int(p+1, pp-(p+1));
			sceIoWrite(out, &buttons, sizeof(buttons));
			*p = 0;
			pp = p;
		}
		buttons = buttons | str2int(buf, sceClibStrnlen(buf, 64));
		sceIoWrite(out,  &buttons, sizeof(buttons));
		sceIoClose(out);
	}
}

void configInputHandler(SceCtrlData *ctrl)
{
	if ((ctrl->buttons & buttons) == buttons)
	{
		sceAppMgrLaunchAppByUri(0x20000, uri);
	}
}

void configInputHandlerNegative(SceCtrlData *ctrl)
{
	if ((ctrl->buttons & buttons) != buttons)
	{
		sceAppMgrLaunchAppByUri(0x20000, uri);
	}
}

// Simplified generic hooking function
void hookFunction(uint32_t nid, const void *func)
{
	hooks[current_hook] = taiHookFunctionImport(&refs[current_hook],TAI_MAIN_MODULE,TAI_ANY_LIBRARY,nid,func);
	current_hook++;
}

int sceCtrlPeekBufferPositive_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[0], port, ctrl, count);
	configInputHandler(ctrl);
	return ret;
}

int sceCtrlPeekBufferPositive2_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[1], port, ctrl, count);
	configInputHandler(ctrl);
	return ret;
}

int sceCtrlReadBufferPositive_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[2], port, ctrl, count);
	configInputHandler(ctrl);
	return ret;
}

int sceCtrlReadBufferPositive2_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[3], port, ctrl, count);
	configInputHandler(ctrl);
	return ret;
}

int sceCtrlPeekBufferPositiveExt_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[4], port, ctrl, count);
	configInputHandler(ctrl);
	return ret;
}

int sceCtrlPeekBufferPositiveExt2_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[5], port, ctrl, count);
	configInputHandler(ctrl);
	return ret;
}

int sceCtrlReadBufferPositiveExt_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[6], port, ctrl, count);
	configInputHandler(ctrl);
	return ret;
}

int sceCtrlReadBufferPositiveExt2_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[7], port, ctrl, count);
	configInputHandler(ctrl);
	return ret;
}

int sceCtrlPeekBufferNegative_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[8], port, ctrl, count);
	configInputHandlerNegative(ctrl);
	return ret;
}

int sceCtrlPeekBufferNegative2_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[9], port, ctrl, count);
	configInputHandlerNegative(ctrl);
	return ret;
}

int sceCtrlReadBufferNegative_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[10], port, ctrl, count);
	configInputHandlerNegative(ctrl);
	return ret;
}

int sceCtrlReadBufferNegative2_patched(int port, SceCtrlData *ctrl, int count)
{
	int ret = TAI_CONTINUE(int, refs[11], port, ctrl, count);
	configInputHandlerNegative(ctrl);
	return ret;
}

void _start() __attribute__ ((weak, alias ("module_start")));
int module_start(SceSize argc, const void *args)
{
	setDefault();
	loadConfig();

	// Hooking functions
	hookFunction(0xA9C3CED6, sceCtrlPeekBufferPositive_patched);
	hookFunction(0x15F81E8C, sceCtrlPeekBufferPositive2_patched);
	hookFunction(0x67E7AB83, sceCtrlReadBufferPositive_patched);
	hookFunction(0xC4226A3E, sceCtrlReadBufferPositive2_patched);
	hookFunction(0xA59454D3, sceCtrlPeekBufferPositiveExt_patched);
	hookFunction(0x860BF292, sceCtrlPeekBufferPositiveExt2_patched);
	hookFunction(0xE2D99296, sceCtrlReadBufferPositiveExt_patched);
	hookFunction(0xA7178860, sceCtrlReadBufferPositiveExt2_patched);
	hookFunction(0x104ED1A7, sceCtrlPeekBufferNegative_patched);
	hookFunction(0x81A89660, sceCtrlPeekBufferNegative2_patched);
	hookFunction(0x15F96FB0, sceCtrlReadBufferNegative_patched);
	hookFunction(0x27A0C5FB, sceCtrlReadBufferNegative2_patched);
	
	return SCE_KERNEL_START_SUCCESS;
}

int module_stop(SceSize argc, const void *args)
{
	// Freeing hooks
	while (current_hook-- > 0){
		taiHookRelease(hooks[current_hook], refs[current_hook]);
	}
		
	return SCE_KERNEL_STOP_SUCCESS;	
}
