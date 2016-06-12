
var m_conn = lg_create_request();

function lg_create_request() {

    if (window.XMLHttpRequest) {

	return new XMLHttpRequest();

    } else if(window.ActiveXObject) {

	return new ActiveXObject("Microsoft.XMLHTTP");
    }

    return null;
}

function lg_send(url, buf, recv_function) {

    m_conn.open("POST", url, true);
    m_conn.onreadystatechange = lg_recv;
    m_conn.lg_function_call = recv_function;
    m_conn.send(buf);
}

function lg_recv() {

    if (m_conn.readyState == 4) {

	if (m_conn.responseText && m_conn.status == 200) {

	    // alert(m_conn.responseText);
	    m_conn.lg_function_call(m_conn.responseText);
	}
    }
}


