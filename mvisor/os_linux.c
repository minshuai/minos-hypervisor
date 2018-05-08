#include <mvisor/mvisor.h>
#include <mvisor/os.h>
#include <mvisor/sched.h>

static void linux_vcpu_init(struct vcpu *vcpu)
{
	/* fill the dtb address to x0 */
	if (get_vcpu_id(vcpu) == 0) {
		vcpu->regs.x0 = vcpu->vm->setup_data;
		vcpu_online(vcpu);
	}
}

static void linux_vcpu_power_on(struct vcpu *vcpu, unsigned long entry)
{
	vcpu->regs.elr_el2 = entry;
	vcpu->regs.x0 = 0;
	vcpu->regs.x1 = 0;
	vcpu->regs.x2 = 0;
	vcpu->regs.x3 = 0;
}

struct os_ops linux_os_ops = {
	.vcpu_init = linux_vcpu_init,
	.vcpu_power_on = linux_vcpu_power_on,
};

static int os_linux_init(void)
{
	struct os *os;

	os = alloc_os("linux");
	if (!os)
		return -EINVAL;

	os->ops = &linux_os_ops;

	return register_os(os);
}

module_initcall(os_linux_init);
