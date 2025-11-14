import ipywidgets as widgets
from IPython.display import display, HTML, Javascript


def create_gui(on_start, on_stop, on_send):
    # === Widgets ===
    send_text = widgets.Text(
        value='',
        description='Message:',
        placeholder='Type message here',
        continuous_update=False,
        layout=widgets.Layout(width='50%')  # Adjust width for better alignment
    )
    send_button = widgets.Button(
        description='Send',
        button_style='success',
        icon='paper-plane',
        layout=widgets.Layout(width='15%')  # Adjust width for better alignment
    )
    start_button = widgets.Button(
        description="Connect",
        button_style='success',
        icon='play',
        layout=widgets.Layout(width='15%')
    )
    stop_button = widgets.Button(
        description="Disconnect",
        button_style='danger',
        icon='stop',
        layout=widgets.Layout(width='15%')
    )

    status_label = widgets.Label(value='Disconnected')

    # === Layout ===
    # Message input and send button
    message_controls = widgets.HBox([send_text, send_button])

    # Start/Stop buttons
    connection_controls = widgets.HBox([start_button, stop_button])

    # Combine everything into a vertical layout
    gui = widgets.VBox([
        widgets.HTML("<h2>KRP - PC Application</h2>"),  # Add a title
        status_label,
        # scrollable_output,  # Use the scrollable wrapper
        message_controls,
        connection_controls
    ])

    # === Event Handlers ===
    def on_send_button_clicked(_):
        message = send_text.value
        if message:
            on_send(message)
            send_text.value = ''  # Clear input field

    send_button.on_click(on_send_button_clicked)
    send_text.on_submit(lambda _: on_send_button_clicked(None))
    start_button.on_click(lambda _: on_start())
    stop_button.on_click(lambda _: on_stop())

    return gui, status_label
