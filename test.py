import subprocess
import re

def extract_times(output):
    print(output)
    user_match = re.search(r'user\s+(\d+)m([\d.]+)s', output)
    sys_match = re.search(r'sys\s+(\d+)m([\d.]+)s', output)
    real_match = re.search(r'real\s+(\d+)m([\d.]+)s', output)

    if user_match and sys_match and real_match:
        user_minutes, user_seconds = int(user_match.group(1)), float(user_match.group(2))
        sys_minutes, sys_seconds = int(sys_match.group(1)), float(sys_match.group(2))
        real_minutes, real_seconds = int(real_match.group(1)), float(real_match.group(2))

        user_time = 60 * user_minutes + user_seconds
        sys_time = 60 * sys_minutes + sys_seconds
        real_time = 60 * real_minutes + real_seconds

        return user_time + sys_time, real_time
    else:
        raise ValueError("User, sys, or real time not found in output")

def run_task_and_get_times(executable):
    command = f'time ./{executable}'
    result = subprocess.run(command, stderr=subprocess.PIPE, stdout=subprocess.DEVNULL, text=True, shell=True)
    return extract_times(result.stderr)

def main():
    executable = 'task_three'
    num_executions = 10
    total_cpu_time = 0
    total_real_time = 0

    for _ in range(num_executions):
        try:
            cpu_time, real_time = run_task_and_get_times(executable)
            total_cpu_time += cpu_time
            total_real_time += real_time
        except Exception as e:
            print(f"Error during execution or parsing: {e}")
            return

    average_cpu_time = total_cpu_time / num_executions
    average_real_time = total_real_time / num_executions

    print(f"Average Real Time over {num_executions} executions: {average_real_time:.3f} seconds")
    print(f"Average CPU Time over {num_executions} executions: {average_cpu_time:.3f} seconds")

if __name__ == "__main__":
    main()
