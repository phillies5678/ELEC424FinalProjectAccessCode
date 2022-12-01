with open('/dev/bone/pwm/1/a/period', 'w') as filetowrite:
    filetowrite.write('20000000')
with open('/dev/bone/pwm/1/a/duty_cycle', 'w') as filetowrite:
    filetowrite.write('1550000')
with open('/dev/bone/pwm/1/a/enable', 'w') as filetowrite:
    filetowrite.write('1')   