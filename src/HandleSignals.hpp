/* **************************************************************************************** */
/*                                                                                          */
/*                                                        ::::::::::: :::::::::   ::::::::  */
/*                                                           :+:     :+:    :+: :+:    :+:  */
/*                                                          +:+     +:+    +:+ +:+          */
/*                                                         +#+     +#++:++#:  +#+           */
/*  By: Timo Saari<tsaari@student.hive.fi>,               +#+     +#+    +#+ +#+            */
/*      Matti Rinkinen<mrinkine@student.hive.fi>,        #+#     #+#    #+# #+#    #+#      */
/*      Marius Meier<mmeier@student.hive.fi>        ########### ###    ###  ########        */
/*                                                                                          */
/* **************************************************************************************** */


// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig);

// Signal handler for SIGTERM (terminate signal)
void handle_sigterm(int sig);

// Signal handler for SIGHUP (reload configuration)
void handle_sighup(int sig);


// Signal handler for SIGSEGV (segmentation fault)
void handle_sigsegv(int sig);

void HandleSignals();