""" Load in saved states """

player_one.focus()

coconut_one.set_weight(1)
coconut_two.set_weight(2)
coconut_three.set_weight(4)
coconut_four.set_weight(8)
coconut_five.set_weight(16)
coconut_six.set_weight(32)
coconut_seven.set_weight(64)

engine.print_terminal(str(coconut_one.get_weight()) + "\n", False)

engine.play_music("beach")

engine.print_terminal(str([a for a in dir(engine) if not a.startswith('__')]) + "\n", False)