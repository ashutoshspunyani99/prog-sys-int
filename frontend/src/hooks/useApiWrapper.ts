import { AxiosResponse } from 'axios';
import { useSnackbar } from '../contexts/SnackbarProvider';
import { ApiResponse } from '../types/ApiResponse';

export function useApiWrapper() {
    const snackbar = useSnackbar();

    return async function ApiWrapper<T>(
        fn: () => Promise<AxiosResponse<ApiResponse<T>>>,
        options?: {
            fallback?: AxiosResponse<ApiResponse<T>>;
            suppressErrors?: boolean;
            context?: string;
            notifySuccess?: boolean;
            notifyFailure?: boolean;
        }
    ): Promise<AxiosResponse<ApiResponse<T>> | undefined> {
        const {
            fallback,
            suppressErrors = false,
            context,
            notifySuccess = true,
            notifyFailure = true,
        } = options || {};

        try {
            const res = await fn();

            const successMessage = res?.data?.message;
            if (notifySuccess && successMessage) {
                console.info(`[${context}] Success: ${res.data.message}`);
                snackbar.showSnackbar(successMessage, 'success');
            }

            return res;
        } catch (err: any) {
            const failureMessage =
                err.response?.data?.message || err.message || 'Unknown error';

            if (!suppressErrors) {
                console.error(`API Error${context ? ` in ${context}` : ''}:`, failureMessage);
            }

            if (notifyFailure) {
                snackbar.showSnackbar(failureMessage, 'error');
            }

            return fallback;
        }
    };
}
